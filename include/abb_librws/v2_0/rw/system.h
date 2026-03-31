#include <abb_librws/common/rw/system.h>

namespace abb :: rws :: v2_0 :: rw :: system
{
    /**
     * \brief A function for retrieving the RobotWare Version.
     *
     * \param RWSResult containing the result from calling the controller API.
     *
     * \return RobotWareVersion describing current firmware.
     */
    RobotWareVersion getRobotWareVersion(RWSClient::RWSResult& result);

}