#include <abb_librws/rws_subscription.h>

#include <sstream>


namespace abb :: rws
{
  using namespace Poco::Net;


  typedef SystemConstants::RWS::Resources     Resources;
  typedef SystemConstants::RWS::Identifiers   Identifiers;
  typedef SystemConstants::RWS::Services      Services;


  /***********************************************************************************************************************
   * Class definitions: SubscriptionResources
   */

  /************************************************************
   * Primary methods
   */

  void SubscriptionResources::addIOSignal(const std::string& iosignal, const SubscriptionPriority priority)
  {
    std::string resource_uri = Resources::RW_IOSYSTEM_SIGNALS;
    resource_uri += "/";
    resource_uri += iosignal;
    resource_uri += ";";
    resource_uri += Identifiers::STATE;

    add(resource_uri, priority);
  }

  void SubscriptionResources::addRAPIDPersistantVariable(const RAPIDResource& resource, const SubscriptionPriority priority)
  {
    std::string resource_uri = Resources::RW_RAPID_SYMBOL_DATA_RAPID;
    resource_uri += "/";
    resource_uri += resource.task;
    resource_uri += "/";
    resource_uri += resource.module;
    resource_uri += "/";
    resource_uri += resource.name;
    resource_uri += ";";
    resource_uri += Identifiers::VALUE;

    add(resource_uri, priority);
  }

  void SubscriptionResources::add(const std::string& resource_uri, const SubscriptionPriority priority)
  {
    resources_.push_back(SubscriptionResource(resource_uri, priority));
  }


  SubscriptionGroup::SubscriptionGroup(POCOClient& client, SubscriptionResources const& resources)
  : client_ {client}
  {
    std::vector<SubscriptionResource> temp = resources.getResources();

    // Generate content for a subscription HTTP post request.
    std::stringstream subscription_content;
    for (std::size_t i = 0; i < temp.size(); ++i)
    {
      subscription_content << "resources=" << i
                            << "&"
                            << i << "=" << temp.at(i).resource_uri
                            << "&"
                            << i << "-p=" << static_cast<int>(temp.at(i).priority)
                            << (i < temp.size() - 1 ? "&" : "");
    }

    // Make a subscription request.
    POCOResult const poco_result = client_.httpPost(Services::SUBSCRIPTION, subscription_content.str());

    if (poco_result.poco_info.http.response.status != HTTPResponse::HTTP_CREATED)
      throw std::runtime_error("Unable to create Subscription: " + poco_result.poco_info.http.response.content);

    subscription_group_id_ = findSubstringContent(poco_result.poco_info.http.response.header_info, "/poll/", "\n");
  }


  SubscriptionGroup::~SubscriptionGroup()
  {
    // Unsubscribe from events
    std::string const uri = Services::SUBSCRIPTION + "/" + subscription_group_id_;
    client_.httpDelete(uri);
  }


  SubscriptionReceiver::SubscriptionReceiver(POCOClient& client, std::string const& subscription_group_id)
  : webSocket_ {client, "/poll/" + subscription_group_id, "robapi2_subscription", DEFAULT_SUBSCRIPTION_TIMEOUT}
  {
  }


  SubscriptionReceiver::~SubscriptionReceiver()
  {
  }
  
  
  bool SubscriptionReceiver::waitForEvent(SubscriptionEvent& event)
  {
    WebSocketFrame frame;
    if (webSocket_.receiveFrame(frame))
    {
      // std::clog << "WebSocket frame received: flags=" << frame.flags << ", frame_content=" << frame.frame_content << std::endl;
      Poco::AutoPtr<Poco::XML::Document> doc = parser_.parseString(frame.frame_content);

      event.value = xmlFindTextContent(doc, XMLAttribute {"class", "lvalue"});

      // IMPORTANT: don't use AutoPtr<XML::Node> here! Otherwise you will get memory corruption.
      if (Poco::XML::Node const * node = doc->getNodeByPath("html/body/div/ul/li/a"))
        event.resourceUri = xmlNodeGetAttributeValue(node, "href");

      return true;
    }
    
    return false;
  }


  void SubscriptionReceiver::forceClose()
  {
    webSocket_.shutdown();
  }


  std::ostream& operator<<(std::ostream& os, SubscriptionEvent const& event)
  {
    return os << "resourceUri=" << event.resourceUri << std::endl
      << "value=" << event.value;
  }
}