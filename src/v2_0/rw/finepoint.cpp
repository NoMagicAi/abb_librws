#include <abb_librws/v2_0/rw/finepoint.h>
#include <string>

namespace abb :: rws :: v2_0 :: rw :: finepoint
{
    std::string FinePointSubscribableResource::getURI() const
    {
        return "NONE";
    }

    void FinePointSubscribableResource::processEvent(Poco::XML::Element const& li_element, std::function<void(SubscriptionEvent const&)> const& callback) const
    {
        FinePointEvent event;
        event.time = -1;
        event.resource = std::make_shared<FinePointSubscribableResource>();
        callback(event);
    }

    int startFinePointSubscription(RWSClient& client)
    {

        // TODO write code here!

        client.httpPut("/rw/_istream/ascii", "", "application/x-www-form-urlencoded;v=2.0");
        client.httpPost("/rw/_istream/undefineall", "");

        POCOResult response = client.httpPost("/rw/_istream/moc", "signal-number=9869&mech-unit=ROB_1&axis-number=0&sample-rate=1&log-period=1");
        int fine_point_subscription_id = -1;
        for (const auto& header : response.headerInfo())
        {
            if (header.first != "Location")
                continue;
            fine_point_subscription_id = std::stoi(header.second.substr(header.second.find_last_of('/') + 1));
            break;
        }

        client.httpPost("/rw/_istream/streamconnect", "");
        client.httpPost("/rw/_istream/startstream", "");
        return fine_point_subscription_id;
    }

    void stopFinePointSubscription(RWSClient& client, int fine_point_subscription_id)
    {
        client.httpPost("/rw/_istream/stopstream", "");
        client.httpDelete("/rw/_istream/moc/" + std::to_string(fine_point_subscription_id));
        client.httpPost("/rw/_istream/undefineall", "");
        client.httpPost("/rw/_istream/streamdisconnect", "");

        // TODO write code here

    }
}