#pragma once

#include <abb_librws/rws_info.h>
#include <abb_librws/v1_0/rws_client.h>

namespace abb :: rws :: v1_0 :: rw :: system
{
    /**
     * \brief A function for retrieving the RobotWare Info.
     *
     * \param result RWSResult containing the result from calling getRobotWareSystem().
     *
     * \return SystemInfo describing current firmware.
     */
    SystemInfo getRobotWareInfo(RWSClient::RWSResult const& result);

}
