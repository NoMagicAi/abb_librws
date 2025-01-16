#include <abb_librws/rws_subscription.h>
#include <abb_librws/rws_error.h>
#include <abb_librws/parsing.h>

#include <Poco/Net/HTTPRequest.h>

#include <boost/exception/diagnostic_information.hpp>

#include <iostream>


namespace abb :: rws
{
  using namespace Poco::Net;

  const std::chrono::microseconds SubscriptionReceiver::WEBSOCKET_UPDATE_INTERVAL = std::chrono::seconds {1};

  // 60 seconds should work for both for RWS1 and RWS2, but since on RWS2 TLS is used and due to a bug in POCO
  // (https://github.com/pocoproject/poco/issues/3848), the actual timeout will be 120 seconds.
  const std::chrono::microseconds SubscriptionReceiver::DEFAULT_SUBSCRIPTION_PING_PONG_TIMEOUT = std::chrono::seconds {60};


  SubscriptionReceiver::SubscriptionReceiver(SubscriptionManager& subscription_manager, AbstractSubscriptionGroup const& group)
  : group_ {group}
  , subscription_manager_ {subscription_manager}
  , webSocket_ {subscription_manager_.receiveSubscription(group.id())}
  , last_ping_time_ {std::chrono::steady_clock::now()}
  {
  }


  SubscriptionReceiver::~SubscriptionReceiver()
  {
  }


  bool SubscriptionReceiver::waitForEvent(std::chrono::microseconds ping_pong_timeout)
  {
    WebSocketFrame frame;
    if (webSocketReceiveFrame(frame, ping_pong_timeout))
    {
      BOOST_LOG_TRIVIAL(debug) << "Processing websocket events: " << frame.frame_content;
      Poco::AutoPtr<Poco::XML::Document> doc = parser_.parseString(frame.frame_content);
      processAllEvents(doc, group_.resources());
      return true;
    }

    return false;
  }


  bool SubscriptionReceiver::webSocketReceiveFrame(WebSocketFrame& frame,
                                                    std::chrono::microseconds ping_pong_timeout)
  {
    if(isShutdown_){
      return false;
    }

    // If the connection is still active...
    int flags = 0;
    std::string content;
    int number_of_bytes_received = 0;
    bool wait_for_data = true;

    // Wait for (non-ping) WebSocket frames.
    do
    {
      auto now = std::chrono::steady_clock::now();
      auto ping_deadline = last_ping_time_ + ping_pong_timeout;
      if (now >= ping_deadline)
        BOOST_THROW_EXCEPTION(TimeoutError {"WebSocket Failed to receive heartbeat message in " +
                                            std::to_string(ping_pong_timeout.count()) + " microseconds."});

      {
        std::lock_guard lock{socketMutex_};
        webSocket_.setReceiveTimeout(WEBSOCKET_UPDATE_INTERVAL);
        flags = 0;

        try
        {
          number_of_bytes_received = webSocket_.receiveFrame(websocket_buffer_, sizeof(websocket_buffer_), flags);
        }
        catch (Poco::TimeoutException const&)
        {
          // Due to the fact that it is not possible to interrupt the receiveFrame() function, when the application
          // is shutting down, the exception is caught and the timeout is handled manually above.
          continue;
        }
      }

      content = std::string(websocket_buffer_, number_of_bytes_received);

      // Check for ping frame.
      if ((flags & WebSocket::FRAME_OP_BITMASK) == WebSocket::FRAME_OP_PING)
      {
        last_ping_time_ = std::chrono::steady_clock::now();
        // Reply with a pong frame.
        std::lock_guard lock{socketMutex_};
        webSocket_.sendFrame(websocket_buffer_,
                                number_of_bytes_received,
                                WebSocket::FRAME_FLAG_FIN | WebSocket::FRAME_OP_PONG);
      }
      else
      {
        wait_for_data = false;
      }
    } while (wait_for_data && !isShutdown_);

    // Check for closing frame.
    if ((flags & WebSocket::FRAME_OP_BITMASK) == WebSocket::FRAME_OP_CLOSE)
    {
      // Do not pass content of a closing frame to end user,
      // according to "The WebSocket Protocol" RFC6455.
      frame.frame_content.clear();
      frame.flags = flags;

      return false;
    }

    frame.flags = flags;
    frame.frame_content = content;

    return number_of_bytes_received != 0;
  }


  void SubscriptionReceiver::shutdown()
  {
    // Shut down the socket. This should make webSocketReceiveFrame() return as soon as possible.
    try{
        isShutdown_ = true;
        std::lock_guard lock{socketMutex_};
        webSocket_.shutdown();
    }catch(const Poco::IOException& ex){
        std::string info = ex.displayText();
        BOOST_LOG_TRIVIAL(error) << "Shutting down websocket failed: " << info;
    }

  }

  void processAllEvents(Poco::AutoPtr<Poco::XML::Document> doc, SubscriptionResources const& resources)
  {
    // IMPORTANT: don't use AutoPtr<XML::Element> here! Otherwise you will get memory corruption.
    Poco::XML::Element const * ul_element = dynamic_cast<Poco::XML::Element const *>(doc->getNodeByPath("html/body/div/ul"));
    if (!ul_element)
      BOOST_THROW_EXCEPTION(ProtocolError {"Cannot parse RWS event message: can't find XML element at path html/body/div/ul"});

    // Cycle through all <li> elements
    Poco::AutoPtr<Poco::XML::NodeList> li_elements = ul_element->getElementsByTagName("li");
    for (unsigned long index = 0; index < li_elements->length(); ++index)
    {
      Poco::XML::Element const * li_element = dynamic_cast<Poco::XML::Element const *>(li_elements->item(index));
      if (!li_element)
        BOOST_THROW_EXCEPTION(std::logic_error {"An item of the list returned by getElementsByTagName() is not an XML::Element"});

      // Cycle throught all subscription resources
      for (auto const& resource : resources)
      {
          resource.processEvent(*li_element);
      }

    }
  }
}