#include <abb_librws/v1_0/rw/ctrl.h>

namespace abb :: rws :: v1_0 :: rw :: ctrl
{
    void restartController(RWSClient& client, RestartMode const& restartMode)
    {
        std::stringstream uri;
        uri << Services::CTRL;

        std::stringstream content;
        content << "restart-mode=" << restartMode;

        client.httpPost(uri.str(), content.str(), std::set<Poco::Net::HTTPResponse::HTTPStatus> {Poco::Net::HTTPResponse::HTTP_NO_CONTENT, Poco::Net::HTTPResponse::HTTP_OK});
    }
}