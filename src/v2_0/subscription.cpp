#include <abb_librws/v2_0/subscription.h>

#include <Poco/Net/HTTPRequest.h>
#include <boost/log/trivial.hpp>


namespace abb :: rws :: v2_0 :: subscription
{
  using namespace Poco::Net;


  SubscriptionGroup::SubscriptionGroup(RWSClient& client, SubscriptionResources const& resources)
  : client_ {client}
  , resources_ {resources}
  , subscription_group_id_ {openSubscription(client_, resources)}
  {
  }


  SubscriptionGroup::SubscriptionGroup(SubscriptionGroup&& rhs) noexcept
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
      try{
        closeSubscription(client_, subscription_group_id_);
      }catch(const abb::rws::CommunicationError& ex){
        std::string info = boost::diagnostic_information(ex);
        BOOST_LOG_TRIVIAL(error) << "Failed to close subscription: " << info;
      }
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


  std::string SubscriptionGroup::resourcesString(RWSClient& client, SubscriptionResources const& resources)
  {
    // Generate content for a subscription HTTP post/put request.
    std::stringstream subscription_content;

    for (std::size_t i = 0; i < resources.size(); ++i)
    {
    subscription_content << "resources=" << i
                          << "&"
                          << i << "=" << resources[i].getURI()
                          << "&"
                          << i << "-p=" << static_cast<int>(resources[i].getPriority())
                          << (i < resources.size() - 1 ? "&" : "");
    }

    return subscription_content.str();
  }


  std::string SubscriptionGroup::openSubscription(RWSClient& client, SubscriptionResources const& resources)
  {
    std::string content_type = "application/x-www-form-urlencoded;v=2.0";

    // Make a subscription request.
    POCOResult const poco_result = client.httpPost(Services::SUBSCRIPTION, resourcesString(client, resources), content_type, {HTTPResponse::HTTP_CREATED});

    std::string subscription_group_id;
    Poco::XML::DOMParser parser;

    // Find "Location" header attribute
    auto const h = std::find_if(
      poco_result.headerInfo().begin(), poco_result.headerInfo().end(),
      [] (auto const& p) { return p.first == "Location"; });

    if (h != poco_result.headerInfo().end())
    {
      std::string const poll = "/poll/";
      auto const start_postion = h->second.find(poll);

      if (start_postion != std::string::npos)
        subscription_group_id = h->second.substr(start_postion + poll.size());
    }

    if (subscription_group_id.empty())
      BOOST_THROW_EXCEPTION(ProtocolError {"Cannot get subscription group from HTTP response"});

    BOOST_LOG_TRIVIAL(debug) << "Received subscription response: " << poco_result.content();
    Poco::AutoPtr<Poco::XML::Document> doc = parser.parseString(poco_result.content());

    return subscription_group_id;
  }


  Poco::AutoPtr<Poco::XML::Document> SubscriptionGroup::updateResources(SubscriptionResources const& res)
  {
    //We skip updating resources to the controller if there are no changes
    if (res == resources_)
    {
      resources_ = res;
      return {};
    }
    POCOResult result = client_.httpPut(Services::SUBSCRIPTION + "/" + subscription_group_id_, resourcesString(client_, res), "application/x-www-form-urlencoded;v=2.0");

    BOOST_LOG_TRIVIAL(debug) << "Received subscription update response: " << result.content();

    resources_ = res;

    if(result.content().empty()) return {};

    return parser_.parseString(result.content());
  }


  void SubscriptionGroup::closeSubscription(RWSClient& client, std::string const& subscription_group_id)
  {
    // Unsubscribe from events
    std::string const uri = Services::SUBSCRIPTION + "/" + subscription_group_id;
    client.httpDelete(uri);
  }
}