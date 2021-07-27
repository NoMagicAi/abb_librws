#pragma once

#include "rws_resource.h"
#include "rws_websocket.h"

#include <Poco/DOM/DOMParser.h>
#include <Poco/Net/WebSocket.h>

#include <string>
#include <iosfwd>
#include <vector>
#include <memory>


namespace abb :: rws
{
  /**
   * \brief An enum for specifying subscription priority.
   */
  enum class SubscriptionPriority
  {
    LOW,    ///< Low priority.
    MEDIUM, ///< Medium priority.
    HIGH    ///< High priority. Only RobotWare 6.05 (or newer) and for IO signals and persistant RAPID variables.
  };


  /**
   * Maps RWS resources to URIs.
   */
  class URIProvider
  {
  public:
    virtual std::string getResourceURI(IOSignalResource const& io_signal) const = 0;
    virtual std::string getResourceURI(RAPIDResource const& resource) const = 0;
  };


  /**
   * \brief Subscription resource that has URI and priority.
   */
  class SubscriptionResource
  {
  public:
    template <typename T>
    SubscriptionResource(T const& resource, SubscriptionPriority priority)
    : resource_ {new ResourceImpl<T> {resource}}
    , priority_ {priority}
    {
    }

    std::string getURI(URIProvider const& uri_gen) const
    {
      return resource_->getURI(uri_gen);
    }

    SubscriptionPriority getPriority() const noexcept
    {
      return priority_;
    }

  private:
    struct ResourceInterface
    {
      virtual std::string getURI(URIProvider const& uri_gen) const = 0;
      virtual ~ResourceInterface() {};
    };

    template <typename T>
    class ResourceImpl
    : public ResourceInterface
    {
    public:
      ResourceImpl(T const& resource)
      : resource_ {resource}
      {
      }

      std::string getURI(URIProvider const& uri_gen) const override
      {
        return uri_gen.getResourceURI(resource_);
      }

    private:
      T resource_;
    };


    std::unique_ptr<ResourceInterface> resource_;

    /**
     * \brief Priority of the subscription.
     */
    SubscriptionPriority priority_;
  };


  using SubscriptionResources = std::vector<SubscriptionResource>;


  /**
   * \brief Contains info about a subscribed event.
   */
  struct SubscriptionEvent
  {
    /**
     * \brief URI of the subscribed resource
     */
    std::string resourceUri;

    /**
     * \brief Value associated with the resource
     */
    std::string value;
  };


  /**
   * \brief Outputs a \a SubscriptionEvent in a human-readable format.
   */
  std::ostream& operator<<(std::ostream& os, SubscriptionEvent const& event);


  /**
   * \brief Receives RWS subscription events.
   */
  class SubscriptionReceiver
  {
  public:
    /**
     * \brief Prepares to receive events from a specified subscription WebSocket.
     *
     * Normally you don't want to construct \a SubscriptionReceiver manually.
     * Use \a SubscriptionGroup::receive() for this.
     *
     * \param websocket A connected WebSocket that should be used to receive events.
     */
    explicit SubscriptionReceiver(Poco::Net::WebSocket&& websocket);


    /**
     * \brief \a SubscriptionReceiver objects are moveable, but not copyable.
     */
    SubscriptionReceiver(SubscriptionReceiver&&) = default;


    /**
     * \brief Closes the WebSocket connection but does not delete the subscription.
     */
    ~SubscriptionReceiver();


    /**
     * \brief Waits for a subscription event.
     *
     * \param event event data
     *
     * \return true if the connection is still alive, false if the connection has been closed.
     */
    bool waitForEvent(SubscriptionEvent& event);


    /**
     * \brief Shutdown the active subscription connection.
     *
     * If waitForEvent() is being executed on a different thread, it will return or throw.
     * It does not delete the subscription from the controller.
     *
     * The preferred way to close the subscription is the destruction of the \a SubscriptionGroup object.
     * This function can be used to force the connection to close immediately in
     * case the robot controller is not responding.
     *
     * This function will return immediately and does not block until an active waitForEvent() has finished.
     *
     */
    void shutdown();


  private:
    /**
     * \brief Default RWS subscription timeout [microseconds].
     */
    static const Poco::Timespan DEFAULT_SUBSCRIPTION_TIMEOUT;

    /**
     * \brief Static constant for the socket's buffer size.
     */
    static const size_t BUFFER_SIZE = 1024;

    /**
     * \brief A buffer for a Subscription.
     */
    char websocket_buffer_[BUFFER_SIZE];

    /**
     * \brief WebSocket for receiving events.
     */
    Poco::Net::WebSocket webSocket_;

    /**
     * \brief Parser for XML in WebSocket frames.
     */
    Poco::XML::DOMParser parser_;


    /**
     * \brief A method for receiving a WebSocket frame.
     *
     * \brief frame the received frame
     *
     * \return true if the connection is still active, false otherwise.
     */
    bool webSocketReceiveFrame(WebSocketFrame& frame);
  };


  class RWSClient;


  /**
   * \brief Manages an RWS subscription group.
   */
  class SubscriptionGroup
  {
  public:
    /**
     * \brief Registers a subscription at the server.
     *
     * \param client a client to subscribe. The lifetime of the client must exceed the lifetime of the subscription group.
     * \param resources list of resources to subscribe
     */
    SubscriptionGroup(RWSClient& client, SubscriptionResources const& resources);


    /**
     * \a SubscriptionGroup objects are moveable, but not copyable.
     */
    SubscriptionGroup(SubscriptionGroup&&) = default;


    /**
     * \brief Ends an active subscription.
     */
    ~SubscriptionGroup();

    /**
     * \brief Get ID of the subscription group.
     *
     * \return ID of the subscription group.
     */
    std::string const& id() const noexcept
    {
      return subscription_group_id_;
    }


    /**
     * \brief Establish WebSocket connection ans start receiving subscription events.
     *
     * \return \a SubscriptionReceiver object that can be used to receive events.
     */
    SubscriptionReceiver receive() const;


  private:
    RWSClient& client_;

    /**
     * \brief A subscription group id.
     */
    std::string subscription_group_id_;
  };
}