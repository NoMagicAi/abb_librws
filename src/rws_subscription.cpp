#include <abb_librws/rws_subscription.h>
#include <abb_librws/rws_error.h>
#include <abb_librws/parsing.h>

#include <Poco/Net/HTTPRequest.h>

#include <boost/exception/diagnostic_information.hpp>

#include <iostream>


namespace abb :: rws
{
  using namespace Poco::Net;


  const std::chrono::microseconds SubscriptionReceiver::DEFAULT_SUBSCRIPTION_TIMEOUT {40000000000};


  SubscriptionReceiver::SubscriptionReceiver(SubscriptionManager& subscription_manager, AbstractSubscriptionGroup const& group)
  : group_ {group}
  , subscription_manager_ {subscription_manager}
  , webSocket_ {subscription_manager_.receiveSubscription(group.id())}
  {
  }


  SubscriptionReceiver::~SubscriptionReceiver()
  {
  }


  bool SubscriptionReceiver::waitForEvent(SubscriptionCallback& callback, std::chrono::microseconds timeout)
  {
    WebSocketFrame frame;
    if (webSocketReceiveFrame(frame, timeout))
    {
      Poco::AutoPtr<Poco::XML::Document> doc = parser_.parseString(frame.frame_content);
      subscription_manager_.processEvent(doc, callback);
      return true;
    }

    return false;
  }


  bool SubscriptionReceiver::webSocketReceiveFrame(WebSocketFrame& frame, std::chrono::microseconds timeout)
  {
    auto now = std::chrono::steady_clock::now();
    auto deadline = std::chrono::steady_clock::now() + timeout;

    // If the connection is still active...
    int flags = 0;
    std::string content;
    int number_of_bytes_received = 0;

    // Wait for (non-ping) WebSocket frames.
    do
    {
      now = std::chrono::steady_clock::now();
      if (now >= deadline)
        BOOST_THROW_EXCEPTION(TimeoutError {"WebSocket frame receive timeout"});

      webSocket_.setReceiveTimeout(std::chrono::duration_cast<std::chrono::microseconds>(deadline - now).count());
      flags = 0;

      try
      {
        number_of_bytes_received = webSocket_.receiveFrame(websocket_buffer_, sizeof(websocket_buffer_), flags);
      }
      catch (Poco::TimeoutException const&)
      {
        BOOST_THROW_EXCEPTION(
          TimeoutError {"WebSocket frame receive timeout"}
            << boost::errinfo_nested_exception(boost::current_exception())
        );
      }

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


  void SubscriptionCallback::processEvent(IOSignalStateEvent const& event)
  {
  }


  void SubscriptionCallback::processEvent(RAPIDExecutionStateEvent const& event)
  {
  }


  void SubscriptionCallback::processEvent(ControllerStateEvent const& event)
  {
  }


  void SubscriptionCallback::processEvent(OperationModeEvent const& event)
  {
  }
}