#pragma once
#include <abb_librws/rws_subscription.h>

namespace abb :: rws :: v2_0 :: rw :: finepoint
{
    struct FinePointSubscribableResource: public SubscribableResource
    {
        explicit FinePointSubscribableResource() = default;

        [[nodiscard]] std::string getURI() const override;

        void processEvent(Poco::XML::Element const& li_element, std::function<void(SubscriptionEvent const&)> const& callback) const override;
    };
}