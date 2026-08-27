
#include <abb_librws/rws.h>
#include <abb_librws/v2_0/rws_client.h>
#include <abb_librws/common/rw/ctrl.h>

#include <string>
#include <iosfwd>

namespace abb :: rws :: v2_0 :: rw
{
    using namespace rws::rw;
}


namespace abb :: rws :: v2_0 :: rw :: ctrl
{
    /**
     * \brief A function for restarting robot.
     *
     * https://developercenter.robotstudio.com/api/RWS?urls.primaryName=Controller%20Service
     *
     * \param client RWS client
     * \param mastership {implicit | explicit} by default mastership is explicit
     * \param restartMode Controller restart type
     *
     * \throw \a RWSError if something goes wrong.
     */
    void restartController(RWSClient& client, Mastership const& mastership, RestartMode const& restartMode = RestartMode::restart);

    /**
     * \brief A function for retrieving the robot controller's info about any safety violation event that potentially is active.
     *
     * \param client RWS client
     *
     * \return RWSResult containing the result.
     *
     * \throw \a RWSError if something goes wrong.
     */
    SafetyViolationInfo getSafetyViolationInfo(RWSClient& client);

    /**
     * \brief A method for loading a SafeMove configuration to the robot controller.
     *
     * \param client RWS client
     * \param resource specifying the file's directory and name.
     *
     * \throw \a std::exception if something goes wrong.
     */
     void loadSafeMoveFile(RWSClient& client, const FileResource& resource);

    /**
     * \brief A function for setting the robot controller's time server.
     *
     * Setting the time server also forces the controller to immediately re-sync against it.
     *
     * \param client RWS client
     * \param time_server_ip IP address of the time server
     *
     * \throw \a RWSError if something goes wrong.
     */
    void setTimeserver(RWSClient& client, std::string const& time_server_ip);
}
