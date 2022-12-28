#pragma once

#include "system_constants.h"

#include <string>
#include <chrono>
#include <list>


namespace abb :: rws
{
    struct ConnectionOptions
    {
        ConnectionOptions(std::string const& ip_address,
            unsigned short port,
            std::string const& username,
            std::string password,
            std::chrono::microseconds connection_timeout = std::chrono::seconds {5},
            std::chrono::microseconds send_timeout = std::chrono::seconds {5},
            std::chrono::microseconds receive_timeout = std::chrono::seconds {5},
            std::list<std::chrono::milliseconds> retry_backoff = std::list<std::chrono::milliseconds> {});

        /// \brief Robot controller's IP address.
        std::string ip_address;

        /// \brief port used by the RWS server.
        unsigned short port;

        /// \brief username to the RWS authentication process.
        std::string username;

        /// \brief password to the RWS authentication process.
        std::string password;

        /// \brief HTTP connection timeout
        std::chrono::microseconds connection_timeout;

        /// \brief HTTP send timeout
        std::chrono::microseconds send_timeout;

        /// \brief HTTP receive timeout
        std::chrono::microseconds receive_timeout;

        /// \brief HTTP backoff time between retry
        std::list<std::chrono::milliseconds> retry_backoff;
    };
}
