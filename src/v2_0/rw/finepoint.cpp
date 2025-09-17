#include <abb_librws/v2_0/rw/finepoint.h>

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

    void startFinePointSubscription(RWSClient& client)
    {
        // TODO write code!
        return;
    }
}