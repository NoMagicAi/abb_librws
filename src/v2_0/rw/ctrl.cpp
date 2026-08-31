#include <abb_librws/v2_0/rw/ctrl.h>

#include <iomanip>

namespace abb :: rws :: v2_0 :: rw :: ctrl
{
    void restartController(RWSClient& client, Mastership const& mastership, RestartMode const& restartMode)
    {
        std::stringstream uri;
        uri << Services::CTRL << "/restart?mastership=" << mastership;

        std::stringstream content;
        content << "restart-mode=" << restartMode;

        std::string content_type = "application/x-www-form-urlencoded;v=2.0";

        client.httpPost(uri.str(), content.str(), content_type);
    }


    SafetyViolationInfo getSafetyViolationInfo(RWSClient& client)
    {
        std::stringstream uri;
        uri << Services::CTRL << "/safety/violation";

        RWSResult rws_result = parseXml(client.httpGet(uri.str()).content());
        
        SafetyViolationInfo result;
        result.unsynchronized = std::stoi(xmlFindTextContent(rws_result, XMLAttribute(Identifiers::CLASS, "unsynchronized"))) != 0;
        result.toolPosViolation = std::stoi(xmlFindTextContent(rws_result, XMLAttribute(Identifiers::CLASS, "tool-pos-violation-status"))) != 0;
        result.armViolation = std::stoi(xmlFindTextContent(rws_result, XMLAttribute(Identifiers::CLASS, "upper-arm-violation-status"))) != 0;
        result.axisRangeViolation = std::stoi(xmlFindTextContent(rws_result, XMLAttribute(Identifiers::CLASS, "axis-range-violation-status"))) != 0;

        return result;
    }


    void loadSafeMoveFile(RWSClient& client, const FileResource& resource)
    {
        std::stringstream uri;
        uri << Services::CTRL << "/safety/config"; ;

        // Path to file should be a direct path, i.e. without "/fileservice/"
        std::string content = "filepath=" + resource.directory + "/" + resource.filename;
        std::string content_type = "application/x-www-form-urlencoded;v=2.0";

        client.httpPost(uri.str(), content, content_type);
    }

    void setTimeserver(RWSClient& client, std::string const& time_server_ip)
    {
        std::string const uri = Services::CTRL + "/clock/timeserver";
        std::string const content = "server-ip=" + time_server_ip;
        std::string const content_type = "application/x-www-form-urlencoded;v=2.0";

        client.httpPost(uri, content, content_type);
    }

    TimeserverInfo getTimeserverInfo(RWSClient& client)
    {
        std::string const uri = Services::CTRL + "/clock/timeserver";

        RWSResult const rws_result = parseXml(client.httpGet(uri).content());

        TimeserverInfo info;
        info.timeserver = xmlFindTextContent(rws_result, XMLAttribute(Identifiers::CLASS, "timeserver"));
        info.time = static_cast<std::time_t>(
            std::stoll(xmlFindTextContent(rws_result, XMLAttribute(Identifiers::CLASS, "time"))));

        return info;
    }

    void setDatetime(RWSClient& client, const int year, const int month, const int day, const int hour, const int minute, const int second)
    {
        std::stringstream content;
        content << "sys-clock-year=" << year
                << "&sys-clock-month=" << std::setw(2) << std::setfill('0') << month
                << "&sys-clock-day=" << std::setw(2) << std::setfill('0') << day
                << "&sys-clock-hour=" << std::setw(2) << std::setfill('0') << hour
                << "&sys-clock-min=" << std::setw(2) << std::setfill('0') << minute
                << "&sys-clock-sec=" << std::setw(2) << std::setfill('0') << second;

        std::string const uri = Services::CTRL + "/clock";
        std::string const content_type = "application/x-www-form-urlencoded;v=2.0";

        client.httpPut(uri, content.str(), content_type);
    }

    void setTimezone(RWSClient& client, std::string const& timezone)
    {
        std::string const uri = Services::CTRL + "/clock/timezone";
        std::string const content = "timezone=" + timezone;
        std::string const content_type = "application/x-www-form-urlencoded;v=2.0";

        client.httpPost(uri, content, content_type);
    }

}
