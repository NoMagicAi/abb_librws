
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

    /**
     * \brief A function for setting the robot controller's time server.
     *
     * \param client RWS client
     * \param time_server_ip IP address of the time server
     *
     * \throw \a RWSError if something goes wrong.
     */
    void setTimeserver(RWSClient& client, std::string const& time_server_ip);

    /**
     * \brief A function for retrieving the robot controller's configured time server and
     * the time it reports for that server.
     *
     * \param client RWS client
     *
     * \return TimeserverInfo containing the configured time server and its reported time.
     *
     * \throw \a RWSError if something goes wrong.
     */
    TimeserverInfo getTimeserverInfo(RWSClient& client);

    /**
     * \brief A function for explicitly setting the robot controller's clock.
     *
     * \param client RWS client
     * \param year four-digit year
     * \param month month of year, 1-12
     * \param day day of month, 1-31
     * \param hour hour of day, 0-23
     * \param minute minute of hour, 0-59
     * \param second second of minute, 0-59
     *
     * \throw \a RWSError if something goes wrong.
     */
    void setDatetime(RWSClient& client, int year, int month, int day, int hour, int minute, int second);

    /**
     * \brief A function for setting the robot controller's timezone.
     *
     * \param client RWS client
     * \param timezone timezone name (e.g. "Europe/Stockholm"), empty string for UTC
     *
     * \throw \a RWSError if something goes wrong.
     */
    void setTimezone(RWSClient& client, std::string const& timezone);

}
