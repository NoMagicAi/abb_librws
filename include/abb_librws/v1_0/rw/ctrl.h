
#include <abb_librws/rws.h>
#include <abb_librws/v1_0/rws_client.h>
#include <abb_librws/common/rw/ctrl.h>

#include <string>
#include <iosfwd>

namespace abb :: rws :: v1_0 :: rw
{
    using namespace rws::rw;
}


namespace abb :: rws :: v1_0 :: rw :: ctrl
{
    /**
     * \brief A function for restarting robot.
     *
     * https://developercenter.robotstudio.com/api/RWS?urls.primaryName=Controller%20Service
     *
     * \param client RWS client
     * \param restartMode Controller restart type
     *
     * \throw \a RWSError if something goes wrong.
     */
    void restartController(RWSClient& client, RestartMode const& restartMode = RestartMode::restart);

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


}
