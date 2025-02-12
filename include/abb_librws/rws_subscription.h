#pragma once

#include "rws_resource.h"
#include "rws_websocket.h"
#include "rws_error.h"

#include <abb_librws/common/rw/rapid.h>
#include <abb_librws/common/rw/panel.h>

#include <Poco/DOM/DOMParser.h>
#include <Poco/DOM/Element.h>
#include <Poco/Net/WebSocket.h>
#include <Poco/DOM/NodeList.h>
#include <Poco/XML/Name.h>

#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <utility>
#include <future>
#include <chrono>
#include <iostream>
#include <functional>
#include <boost/log/trivial.hpp>


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


  class SubscriptionCallback;
  class AbstractSubscriptionGroup;
  struct SubscriptionEvent;

  struct SubscribableResource
  {
    public:
        virtual std::string getURI() const = 0;
        virtual void processEvent(Poco::XML::Element const& li_element, std::function<void(SubscriptionEvent const&)> const& callback) const = 0;

        bool equals(const SubscribableResource& rhs) const
        {
          return getURI() == rhs.getURI();
        }

        std::size_t getHash() const
        { return std::hash<std::string>()(getURI()); }
  };


  /**
   * \brief Provides the mechanism to open, receive, and close RWS event subscription.
   */
  class SubscriptionManager
  {
  public:

    /**
     * \brief Open a WebSocket and start receiving subscription events.
     *
     * \param subscription_group_id subscription group id for which to receive event.
     *
     * \return WebSocket created to receive the events.
     *
     * \throw \a RWSError if something goes wrong.
     */
    virtual Poco::Net::WebSocket receiveSubscription(std::string const& subscription_group_id) = 0;
  };

    struct SubscriptionEvent{
        virtual ~SubscriptionEvent() = default;
        std::shared_ptr<SubscribableResource> resource;
    };


  /**
   * \brief Subscription resource that has URI and priority.
   */
  class SubscriptionResource
  {
  public:
    SubscriptionResource(std::shared_ptr<SubscribableResource> resource_ptr, SubscriptionPriority priority, std::function<void(SubscriptionEvent const&)> callback)
    : resource_ptr_ {std::move(resource_ptr)}
    , priority_ {priority}
    , callback_ {std::move(callback)}
    {
    }

    [[nodiscard]] std::string getURI() const
    {
      return resource_ptr_ -> getURI();
    }

    [[nodiscard]] SubscriptionPriority getPriority() const noexcept
    {
      return priority_;
    }

    void processEvent(Poco::XML::Element const& li_element) const
    {
      resource_ptr_ -> processEvent(li_element, callback_);
    }

    bool operator == ( SubscriptionResource const& other ) const noexcept
    {
      return getURI() == other.getURI() && getPriority() == other.getPriority();
    }

  private:

    std::shared_ptr<SubscribableResource> resource_ptr_;

    /**
     * \brief Priority of the subscription.
     */
    SubscriptionPriority priority_;
      /**
   * \brief Callback to be called when the subscription event arrives.
   */
    std::function<void(SubscriptionEvent const&)> callback_;
  };


  using SubscriptionResources = std::vector<SubscriptionResource>;


  /**
   * \brief Event received when an IO signal state changes.
   */
  struct IOSignalStateEvent: public SubscriptionEvent
  {
    /// \brief IO signal name
    std::string signal;

    /**
     * \brief IO signal value
     */
    std::string value;
  };


  /**
   * \brief Event received when an IO signal state changes.
   */
  struct RAPIDExecutionStateEvent: public SubscriptionEvent
  {
    /**
     * \brief RAPID execution state
     */
    rw::RAPIDExecutionState state;
  };

    /**
     * \brief Event received when a speed ratio changes
     */
    struct SpeedRatioChangedEvent: public SubscriptionEvent
    {
        /**
         * \brief New value of the speed ratio
         */
        unsigned int value;
    };


  /**
   * \brief Event received when controller state changes.
   */
  struct ControllerStateEvent: public SubscriptionEvent
  {
    /**
     * \brief Controller state
     */
    rw::ControllerState state;
  };


  /**
   * \brief Event received when operation mode changes.
   */
  struct OperationModeEvent: public SubscriptionEvent
  {
    /**
     * \brief Operation mode
     */
    rw::OperationMode mode;
  };

  /**
   * \brief Event recieved when new event is added to elog.
   */
  struct ElogEvent: public SubscriptionEvent
  {
    int domain; // domain number
    int seqnum; // seqnence numver of message in domain
  };


  /**
   * \brief Receives RWS subscription events.
   */
  class SubscriptionReceiver
  {
  public:
    /**
     * \brief Prepares to receive events from a specified subscription WebSocket.
     *
     * \param subscription_manager used to initiate WebSocket connection and parse incomoing message content
     * \param group subscription group for which to receive events
     */
    explicit SubscriptionReceiver(SubscriptionManager& subscription_manager, AbstractSubscriptionGroup const& group);


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
     * \param callback callback to be called when an event arrives
     * \param timeout wait timeout
     *
     * \return true if the connection is still alive, false if the connection has been closed.
     *
     * \throw \a TimeoutError if waiting time exceeds \a timeout.
     */
    bool waitForEvent(std::chrono::microseconds ping_pong_timeout = DEFAULT_SUBSCRIPTION_PING_PONG_TIMEOUT);


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
     * \brief Default RWS subscription timeout for waiting for a new message to arrive [microseconds].
     */
    static const std::chrono::microseconds WEBSOCKET_UPDATE_INTERVAL;

    /**
     * \brief Default RWS subscription timeout for ping-pong to arrive [microseconds].
     */
    static const std::chrono::microseconds DEFAULT_SUBSCRIPTION_PING_PONG_TIMEOUT;

    /**
     * \brief Static constant for the socket's buffer size.
     */
    static const size_t BUFFER_SIZE = 1024;

    // Subscription group to which the SubscriptionReceiver is bound.
    AbstractSubscriptionGroup const& group_;

    /**
     * \brief A buffer for a Subscription.
     */
    char websocket_buffer_[BUFFER_SIZE];

    SubscriptionManager& subscription_manager_;

    /**
     * \brief WebSocket for receiving events.
     */
    Poco::Net::WebSocket webSocket_;

    /**
     * \brief Parser for XML in WebSocket frames.
     */
    Poco::XML::DOMParser parser_;

    bool webSocketReceiveFrame(WebSocketFrame& frame, std::chrono::microseconds ping_pong_timeout);

    /**
     * \brief Last time a ping-pong message was received.
     */
    std::chrono::time_point<std::chrono::steady_clock> last_ping_time_;

    /**
     * \brief Flag to indicate if the SubscriptionReceiver is shutting down.
     */
    std::atomic<bool> isShutdown_ = false;

    /**
     * \brief Mutex to guard webSocket_ in between threads.
     */
    std::mutex socketMutex_;
  };


  /**
   * \brief Abstract RWS subscription group.
   */
  class AbstractSubscriptionGroup
  {
  public:
    virtual ~AbstractSubscriptionGroup() noexcept = default;

    /**
     * \brief Get ID of the subscription group.
     *
     * \return ID of the subscription group.
     */
    virtual std::string const& id() const noexcept = 0;

    /**
     * @brief Get subscribed resources.
     *
     * @return list of subscribed resources.
     */
    virtual SubscriptionResources const& resources() const noexcept = 0;

    /**
     * @brief Update subscribed resources.
     *
     * @param res list of new subscribed resources.
     */
    virtual Poco::AutoPtr<Poco::XML::Document> updateResources(SubscriptionResources const& res) = 0;

    /**
     * \brief Establish WebSocket connection ans start receiving subscription events.
     *
     * \return \a SubscriptionReceiver object that can be used to receive events.
     */
    virtual SubscriptionReceiver receive() const = 0;

    /**
     * \brief Close the subscription.
     *
     * Closes the subscription as if the destructor was called.
     * The \a id() will become empty and the subsequent calls to \a receive() will fail.
     * This function can be used to close the subscription before destroying the object,
     * and to catch possible errors, which would be problematic with the destructor.
     *
     * \throw \a RWSError if something goes wrong.
     */
    virtual void close() = 0;

    /**
     * \brief Detach the object from the subscription group.
     *
     * Detaches the object from the actual subscription group, without attempting to close it.
     * The \a id() will become empty and the subsequent calls to \a receive() will fail.
     * This is useful if closing the subscription group fails, but you want to continue.
     */
    virtual void detach() noexcept = 0;
  };

    /**
   * \brief Process all events in a subscription package.
   *
   * Parses all events in \a content, determines event type, and calls the appropriate functions in \a callback.
   *
   * \param content XML content of the event
   * \param callback event callback
   */
  void processAllEvents(Poco::AutoPtr<Poco::XML::Document> doc, SubscriptionResources const& resources);
}