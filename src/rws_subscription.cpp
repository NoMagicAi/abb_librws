#include <abb_librws/rws_subscription.h>
#include <abb_librws/rws_error.h>
#include <abb_librws/rws_client.h>

#include <Poco/Net/HTTPRequest.h>

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


  SubscriptionGroup::SubscriptionGroup(RWSClient& client, SubscriptionResources const& resources)
  : client_ {client}
  , subscription_group_id_ {client.openSubscription(resources)}
  {
  }


  SubscriptionGroup::~SubscriptionGroup()
  {
    client_.closeSubscription(subscription_group_id_);
  }


  SubscriptionReceiver SubscriptionGroup::receive() const
  {
    return SubscriptionReceiver {client_.receiveSubscription(subscription_group_id_)};
  }


  const Poco::Timespan SubscriptionReceiver::DEFAULT_SUBSCRIPTION_TIMEOUT {40000000000};


  SubscriptionReceiver::SubscriptionReceiver(Poco::Net::WebSocket&& websocket)
  : webSocket_ {websocket}
  {
    webSocket_.setReceiveTimeout(DEFAULT_SUBSCRIPTION_TIMEOUT);
  }


  SubscriptionReceiver::~SubscriptionReceiver()
  {
  }


  bool SubscriptionReceiver::waitForEvent(SubscriptionEvent& event)
  {
    WebSocketFrame frame;
    if (webSocketReceiveFrame(frame))
    {
      Poco::AutoPtr<Poco::XML::Document> doc = parser_.parseString(frame.frame_content);

      event.value = xmlFindTextContent(doc, XMLAttribute {"class", "lvalue"});

      // IMPORTANT: don't use AutoPtr<XML::Node> here! Otherwise you will get memory corruption.
      if (Poco::XML::Node const * node = doc->getNodeByPath("html/body/div/ul/li/a"))
        event.resourceUri = xmlNodeGetAttributeValue(node, "href");

      return true;
    }

    return false;
  }


  bool SubscriptionReceiver::webSocketReceiveFrame(WebSocketFrame& frame)
  {
    // If the connection is still active...
    int flags = 0;
    std::string content;
    int number_of_bytes_received = 0;

    // Wait for (non-ping) WebSocket frames.
    do
    {
      flags = 0;
      number_of_bytes_received = webSocket_.receiveFrame(websocket_buffer_, sizeof(websocket_buffer_), flags);
      content = std::string(websocket_buffer_, number_of_bytes_received);

      // Check for ping frame.
      if ((flags & WebSocket::FRAME_OP_BITMASK) == WebSocket::FRAME_OP_PING)
      {
        // Reply with a pong frame.
        webSocket_.sendFrame(websocket_buffer_,
                                number_of_bytes_received,
                                WebSocket::FRAME_FLAG_FIN | WebSocket::FRAME_OP_PONG);
      }
    } while ((flags & WebSocket::FRAME_OP_BITMASK) == WebSocket::FRAME_OP_PING);

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
    webSocket_.shutdown();
  }


  std::ostream& operator<<(std::ostream& os, SubscriptionEvent const& event)
  {
    return os << "resourceUri=" << event.resourceUri << std::endl
      << "value=" << event.value;
  }
}