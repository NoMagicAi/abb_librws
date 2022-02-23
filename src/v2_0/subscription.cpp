#include <abb_librws/v2_0/subscription.h>


namespace abb :: rws :: v2_0 :: subscription
{
  using namespace Poco::Net;


  SubscriptionGroup::SubscriptionGroup(RWSClient& client, SubscriptionResources const& resources)
  : client_ {client}
  , resources_ {resources}
  , subscription_group_id_ {client_.openSubscription(getURI(resources))}
  {
  }


  SubscriptionGroup::SubscriptionGroup(SubscriptionGroup&& rhs)
  : client_ {rhs.client_}
  , resources_ {std::move(rhs.resources_)}
  , subscription_group_id_ {rhs.subscription_group_id_}
  {
    // Clear subscription_group_id_ of the SubscriptionGroup that has been moved from,
    // s.t. its destructor does not close the subscription.
    rhs.subscription_group_id_.clear();
  }


  SubscriptionGroup::~SubscriptionGroup()
  {
    close();
  }


  void SubscriptionGroup::close()
  {
    if (!subscription_group_id_.empty())
    {
      client_.closeSubscription(subscription_group_id_);
      subscription_group_id_.clear();
    }
  }


  void SubscriptionGroup::detach() noexcept
  {
    subscription_group_id_.clear();
  }


  SubscriptionReceiver SubscriptionGroup::receive() const
  {
    return SubscriptionReceiver {client_, *this};
  }


  std::vector<std::pair<std::string, SubscriptionPriority>> SubscriptionGroup::getURI(SubscriptionResources const& resources)
  {
    std::vector<std::pair<std::string, SubscriptionPriority>> uri;
    uri.reserve(resources.size());

    for (auto&& r : resources)
      uri.emplace_back(r.getURI(), r.getPriority());

    return uri;
  }
}