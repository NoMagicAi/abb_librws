#pragma once

#include "system_constants.h"

#include <string>
#include <chrono>

namespace abb ::rws
{
struct ConnectionOptions
{
  ConnectionOptions(std::string const& ip_address, unsigned short port = SystemConstants::General::DEFAULT_PORT_NUMBER,
                    std::string const& username = SystemConstants::General::DEFAULT_USERNAME,
                    std::string password = SystemConstants::General::DEFAULT_PASSWORD,
                    std::chrono::microseconds connection_timeout = std::chrono::milliseconds{ 400 },
                    std::chrono::microseconds send_timeout = std::chrono::milliseconds{ 400 },
                    std::chrono::microseconds receive_timeout = std::chrono::milliseconds{ 400 });

  /// \brief Robot controller's IP address.
  std::string ip_address;

  /// \brief port used by the RWS server.
  unsigned short port;

  /// \brief username to the RWS authentication process.
  std::string username;

  /// \brief password to the RWS authentication process.
  std::string password;

  /// \brief HTTP connection timeout
  std::chrono::microseconds connection_timeout = std::chrono::milliseconds{ 400 };

  /// \brief HTTP send timeout
  std::chrono::microseconds send_timeout = std::chrono::milliseconds{ 400 };

  /// \brief HTTP receive timeout
  std::chrono::microseconds receive_timeout = std::chrono::milliseconds{ 400 };
};
}  // namespace abb::rws
