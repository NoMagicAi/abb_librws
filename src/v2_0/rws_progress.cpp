#include <abb_librws/v2_0/rws_progress.h>


namespace abb :: rws :: v2_0 :: progress
{
    ProgressInfo getProgress(RWSClient& client, std::string const& id)
    {
        std::string uri = "/progress/" + id;
        RWSResult rws_result = parseXml(client.httpGet(uri,{Poco::Net::HTTPResponse::HTTP_OK, Poco::Net::HTTPResponse::HTTP_CREATED}).content());
        ProgressInfo progress_info;
        progress_info.code = std::stoi(xmlNodeTextByTagAndAttribute(rws_result, "span", abb::rws::XMLAttribute("class","code")).value());
        progress_info.state = xmlNodeTextByTagAndAttribute(rws_result, "span", abb::rws::XMLAttribute("class","state")).value();
        return progress_info;
    }

    const std::string ProgressState::READY = "ready";
    const std::string ProgressState::PENDING = "pending";
}
