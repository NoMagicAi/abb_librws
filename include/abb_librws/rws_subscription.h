#pragma once

#include "rws_resource.h"
#include "rws_websocket.h"
#include "rws_error.h"

#include <abb_librws/common/rw/rapid.h>
#include <abb_librws/common/rw/panel.h>

#include <Poco/DOM/DOMParser.h>
#include <Poco/DOM/Element.h>
#include <Poco/Net/WebSocket.h>

#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <future>
#include <chrono>


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

    std::string getURI() const
    {
      return resource_->getURI();
    }

    void processEvent(Poco::XML::Element const& li_element, SubscriptionCallback& callback) const
    {
      resource_->processEvent(li_element, callback);
    }

    SubscriptionPriority getPriority() const noexcept
    {
      return priority_;
    }

  private:
    struct ResourceInterface
    {
      virtual std::string getURI() const = 0;
      virtual void processEvent(Poco::XML::Element const& li_element, SubscriptionCallback& callback) const = 0;
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

      std::string getURI() const override
      {
        return resource_.getURI();
      }

      void processEvent(Poco::XML::Element const& li_element, SubscriptionCallback& callback) const override
      {
        resource_.processEvent(li_element, callback);
      }

    private:
      T const resource_;
    };


    std::shared_ptr<ResourceInterface> resource_;

    /**
     * \brief Priority of the subscription.
     */
    SubscriptionPriority priority_;
  };


  using SubscriptionResources = std::vector<SubscriptionResource>;


  /**
   * \brief Event received when an IO signal state changes.
   */
  struct IOSignalStateEvent
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
  struct RAPIDExecutionStateEvent
  {
    /**
     * \brief RAPID execution state
     */
    rw::RAPIDExecutionState state;
  };


  /**
   * \brief Event received when controller state changes.
   */
  struct ControllerStateEvent
  {
    /**
     * \brief Controller state
     */
    rw::ControllerState state;
  };


  /**
   * \brief Event received when operation mode changes.
   */
  struct OperationModeEvent
  {
    /**
     * \brief Operation mode
     */
    rw::OperationMode mode;
  };


  /**
   * \brief Defines callbacks for different types of RWS subscription events.
   */
  class SubscriptionCallback
  {
  public:
    virtual void processEvent(RAPIDExecutionStateEvent const& event);
    virtual void processEvent(ControllerStateEvent const& event);
    virtual void processEvent(OperationModeEvent const& event);
  };


  /**
   * \brief Abstract RWS subscription receiver.
   */
  class AbstractSubscriptionReceiver
  {
  protected:
    /**
     * \brief Prepares to receive events from a specified subscription WebSocket.
     *
     * \param web_socket an open WebSocket to receive RWS events
     * \param group subscription group for which to receive events
     */
    explicit AbstractSubscriptionReceiver(Poco::Net::WebSocket&& web_socket, AbstractSubscriptionGroup const& group);


    /**
     * \brief \a SubscriptionReceiver objects are moveable, but not copyable.
     */
    AbstractSubscriptionReceiver(AbstractSubscriptionReceiver&&) = default;


  public:
    /**
     * \brief Closes the WebSocket connection but does not delete the subscription.
     */
    virtual ~AbstractSubscriptionReceiver();


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
    bool waitForEvent(SubscriptionCallback& callback, std::chrono::microseconds timeout = DEFAULT_SUBSCRIPTION_TIMEOUT);


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
    static const std::chrono::microseconds DEFAULT_SUBSCRIPTION_TIMEOUT;

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

    /**
     * \brief WebSocket for receiving events.
     */
    Poco::Net::WebSocket webSocket_;

    /**
     * \brief Parser for XML in WebSocket frames.
     */
    Poco::XML::DOMParser parser_;


    bool webSocketReceiveFrame(WebSocketFrame& frame, std::chrono::microseconds timeout);
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
    virtual void resources(SubscriptionResources const& res) = 0;

    /**
     * \brief Establish WebSocket connection ans start receiving subscription events.
     *
     * \return pointer to \a AbstractSubscriptionReceiver that can be used to receive events.
     */
    virtual std::unique_ptr<AbstractSubscriptionReceiver> receive() const = 0;

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
   * \brief Wait for a subscription event of a specific type.
   *
   * \tparam T type of an event to wait for
   *
   * \param receiver RWS subscription receiver
   * \param timeout wait timeout
   *
   * \return \a std::future with the received event.
   *
   * \throw \a CommunicationError if the subscription WebSocket connection is closed while waiting for the event.
   * \throw \a TimeoutError if waiting time exceeds \a timeout.
   */
  template <typename T>
  inline std::future<T> waitForEvent(AbstractSubscriptionReceiver& receiver, std::chrono::microseconds timeout)
  {
    struct Callback : rws::SubscriptionCallback
    {
        void processEvent(T const& event) override
        {
            event_ = event;
        }

        T event_;
    };


    return std::async(std::launch::async,
        [&receiver, timeout] {
            Callback callback;
            if (!receiver.waitForEvent(callback, timeout))
              BOOST_THROW_EXCEPTION(CommunicationError {"WebSocket connection shut down when waiting for a subscription event"});
            return callback.event_;
        }
    );
  }


  /**
   * \brief Wait for a subscription event of any type.
   *
   * \param receiver RWS subscription receiver
   * \param timeout wait timeout
   *
   * \return \a std::future with the received event.
   *
   * \throw \a CommunicationError if the subscription WebSocket connection is closed while waiting for the event.
   * \throw \a TimeoutError if waiting time exceeds \a timeout.
   */
  inline std::future<void> waitForEvent(AbstractSubscriptionReceiver& receiver, std::chrono::microseconds timeout)
  {
    return std::async(std::launch::async,
        [&receiver, timeout] {
            rws::SubscriptionCallback callback;
            if (!receiver.waitForEvent(callback, timeout))
              BOOST_THROW_EXCEPTION(CommunicationError {"WebSocket connection shut down when waiting for a subscription event"});
        }
    );
  }


  /**
   * \brief Process all events in a subscription package.
   *
   * Parses all events in \a content, determines event type, and calls the appropriate functions in \a callback.
   *
   * \param content XML content of the event
   * \param callback event callback
   */
  void processAllEvents(Poco::AutoPtr<Poco::XML::Document> doc, SubscriptionResources const& resources, SubscriptionCallback& callback);
}