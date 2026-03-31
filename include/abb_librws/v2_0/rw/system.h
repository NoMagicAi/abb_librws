#include <abb_librws/common/rw/system.h>

namespace abb :: rws :: v2_0 :: rw :: system
{
    /**
     * \brief A function for retrieving the RobotWare Info.
     *
     * \param RWSResult containing the result from calling the controller API.
     *
     * \return RobotWareInfo describing current firmware.
     */
    SystemInfo getRobotWareInfo(RWSClient::RWSResult& result);

}