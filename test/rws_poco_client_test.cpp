#include <gtest/gtest.h>

#include <abb_librws/rws_error.h>
#include <abb_librws/rws_poco_client.h>

#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPCookie.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPServerParams.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/NameValueCollection.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Timespan.h>

#include <chrono>
#include <deque>
#include <mutex>
#include <string>
#include <thread>
#include <utility>
#include <vector>

namespace abb :: rws
{
    namespace
    {
        using Poco::Net::HTTPResponse;

        char const* const SESSION_COOKIE = "-http-session-";
        char const* const CLIENT_COOKIE = "ABBCX";

        Poco::Timespan const RECEIVE_TIMEOUT {0, 200 * 1000};
        // Longer than RECEIVE_TIMEOUT, so that the client gives up before the response comes.
        std::chrono::milliseconds const SLOW_RESPONSE_DELAY {400};

        /**
         * \brief The response of the fake RWS server to one request.
         */
        struct ScriptedResponse
        {
            HTTPResponse::HTTPStatus status;
            std::vector<std::pair<std::string, std::string>> set_cookies;
            std::chrono::milliseconds delay {0};
        };

        /**
         * \brief What the fake RWS server received in one request.
         */
        struct ReceivedRequest
        {
            std::string session_cookie;
            bool has_credentials;
        };

        /**
         * \brief A response that sets the cookies of a new session, as the controller does after a login.
         */
        ScriptedResponse login(std::string const& session)
        {
            return {HTTPResponse::HTTP_OK, {{SESSION_COOKIE, session}, {CLIENT_COOKIE, "1"}}};
        }

        /**
         * \brief An HTTP server on the loopback interface that answers each request with the next scripted response.
         *
         * A 401 response has the WWW-Authenticate header of an OmniCore controller, so the client answers it with Basic
         * credentials. After the last scripted response, each request gets 500.
         */
        class FakeRwsServer
        {
        public:
            explicit FakeRwsServer(std::deque<ScriptedResponse> responses)
            : responses_ {std::move(responses)}
            , socket_ {Poco::Net::SocketAddress {"127.0.0.1", 0}}
            , server_ {new HandlerFactory {*this}, socket_, new Poco::Net::HTTPServerParams}
            {
                server_.start();
            }

            ~FakeRwsServer()
            {
                server_.stopAll(true);
            }

            unsigned short port() const
            {
                return socket_.address().port();
            }

            std::vector<ReceivedRequest> requests() const
            {
                std::lock_guard<std::mutex> lock {mutex_};
                return requests_;
            }

        private:
            class Handler : public Poco::Net::HTTPRequestHandler
            {
            public:
                explicit Handler(FakeRwsServer& server)
                : server_ {server}
                {
                }

                void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override
                {
                    ScriptedResponse const scripted = server_.record(request);
                    std::this_thread::sleep_for(scripted.delay);

                    for (auto const& [name, value] : scripted.set_cookies)
                    {
                        Poco::Net::HTTPCookie cookie {name, value};
                        cookie.setPath("/");
                        response.addCookie(cookie);
                    }

                    if (scripted.status == HTTPResponse::HTTP_UNAUTHORIZED)
                    {
                        response.set("WWW-Authenticate", "Basic realm=\"validusers@robapi.abb\"");
                    }

                    response.setStatus(scripted.status);
                    response.setContentLength(0);
                    response.send();
                }

            private:
                FakeRwsServer& server_;
            };

            class HandlerFactory : public Poco::Net::HTTPRequestHandlerFactory
            {
            public:
                explicit HandlerFactory(FakeRwsServer& server)
                : server_ {server}
                {
                }

                Poco::Net::HTTPRequestHandler* createRequestHandler(Poco::Net::HTTPServerRequest const&) override
                {
                    return new Handler {server_};
                }

            private:
                FakeRwsServer& server_;
            };

            ScriptedResponse record(Poco::Net::HTTPServerRequest const& request)
            {
                Poco::Net::NameValueCollection cookies;
                request.getCookies(cookies);

                std::lock_guard<std::mutex> lock {mutex_};
                requests_.push_back({cookies.get(SESSION_COOKIE, ""), request.hasCredentials()});

                if (responses_.empty())
                {
                    return {HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, {}};
                }

                ScriptedResponse const scripted = responses_.front();
                responses_.pop_front();
                return scripted;
            }

            mutable std::mutex mutex_;
            std::deque<ScriptedResponse> responses_;
            std::vector<ReceivedRequest> requests_;
            Poco::Net::ServerSocket socket_;
            Poco::Net::HTTPServer server_;
        };

        /**
         * \brief A POCOClient with its own HTTP session to a server on the loopback interface.
         */
        struct ClientConnection
        {
            explicit ClientConnection(unsigned short port)
            : session {"127.0.0.1", port}
            , client {session, "Default User", "robotics"}
            {
                session.setTimeout(RECEIVE_TIMEOUT);
            }

            Poco::Net::HTTPClientSession session;
            POCOClient client;
        };
    }

    TEST(POCOClientTest, keepsTheSessionAfterAReceiveTimeout)
    {
        FakeRwsServer server {{
            login("s1"),
            {HTTPResponse::HTTP_OK, {}, SLOW_RESPONSE_DELAY},
            {HTTPResponse::HTTP_OK, {}},
        }};
        ClientConnection connection {server.port()};

        connection.client.httpGet("/rw/system");
        EXPECT_THROW(connection.client.httpGet("/rw/system"), CommunicationError);
        connection.client.httpGet("/rw/system");

        auto const requests = server.requests();
        ASSERT_EQ(requests.size(), 3u);
        EXPECT_EQ(requests[2].session_cookie, "s1");
        EXPECT_FALSE(requests[2].has_credentials);
    }

    TEST(POCOClientTest, keepsTheSessionAfterAServerError)
    {
        FakeRwsServer server {{
            login("s1"),
            {HTTPResponse::HTTP_SERVICE_UNAVAILABLE, {}},
            {HTTPResponse::HTTP_OK, {}},
        }};
        ClientConnection connection {server.port()};

        connection.client.httpGet("/rw/system");
        EXPECT_EQ(connection.client.httpGet("/rw/system").httpStatus(), HTTPResponse::HTTP_SERVICE_UNAVAILABLE);
        connection.client.httpGet("/rw/system");

        auto const requests = server.requests();
        ASSERT_EQ(requests.size(), 3u);
        EXPECT_EQ(requests[2].session_cookie, "s1");
        EXPECT_FALSE(requests[2].has_credentials);
    }

    TEST(POCOClientTest, logsInToANewSessionWhenTheSessionIsGone)
    {
        FakeRwsServer server {{
            login("s1"),
            {HTTPResponse::HTTP_UNAUTHORIZED, {}},
            login("s2"),
            {HTTPResponse::HTTP_OK, {}},
        }};
        ClientConnection connection {server.port()};

        connection.client.httpGet("/rw/system");
        EXPECT_EQ(connection.client.httpGet("/rw/system").httpStatus(), HTTPResponse::HTTP_OK);
        connection.client.httpGet("/rw/system");

        auto const requests = server.requests();
        ASSERT_EQ(requests.size(), 4u);
        EXPECT_TRUE(requests[2].has_credentials);
        EXPECT_EQ(requests[3].session_cookie, "s2");
        EXPECT_FALSE(requests[3].has_credentials);
    }

    TEST(POCOClientTest, keepsTheSessionWhenTheLoginSetsNoCookies)
    {
        FakeRwsServer server {{
            login("s1"),
            {HTTPResponse::HTTP_UNAUTHORIZED, {}},
            {HTTPResponse::HTTP_OK, {}},
            {HTTPResponse::HTTP_OK, {}},
        }};
        ClientConnection connection {server.port()};

        connection.client.httpGet("/rw/system");
        EXPECT_EQ(connection.client.httpGet("/rw/system").httpStatus(), HTTPResponse::HTTP_OK);
        connection.client.httpGet("/rw/system");

        auto const requests = server.requests();
        ASSERT_EQ(requests.size(), 4u);
        EXPECT_EQ(requests[3].session_cookie, "s1");
    }
}
