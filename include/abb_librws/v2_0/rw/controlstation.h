#pragma once
#include <string>

#include <abb_librws/v2_0/rws_client.h>

namespace abb ::rws ::v2_0 ::rw ::controlstation {
/**
 * \brief A class for operating on controlstation in ABB controller (RobotWare >= 8).
 *
 * https://developercenter.robotstudio.com/api/RWS?urls.primaryName=ControlStation%20Service
 */
class ControlStationInterface {
 public:
  explicit ControlStationInterface(RWSClient& rws_client);

  /**
   * \brief Register as remote control station.
   *
   * https://developercenter.robotstudio.com/api/RWS?urls.primaryName=ControlStation%20Service
   *
   * \param control_station_name name of the remote control station.
   * \param control_station_id unique identifier (UUID format).
   * \param pincode numeric security code.
   * \param release_write_access_when_lost whether to auto-release write access when connection is lost.
   *
   * \throw \a RWSError if something goes wrong.
   */
  void registerRemote(std::string const& control_station_name,
                      std::string const& control_station_id,
                      std::string const& pincode,
                      bool release_write_access_when_lost = true) const;

  /**
   * \brief Request write access from the robot controller.
   *
   * Must be registered as a control station first.
   *
   * https://developercenter.robotstudio.com/api/RWS?urls.primaryName=ControlStation%20Service
   *
   * \throw \a RWSError if something goes wrong.
   */
  void requestWriteAccess() const;

  /**
   * \brief Release the write access currently held by this control station.
   *
   * https://developercenter.robotstudio.com/api/RWS?urls.primaryName=ControlStation%20Service
   *
   * \throw \a RWSError if something goes wrong.
   */
  void releaseWriteAccess() const;

  /**
   * \brief Check the current write access status.
   *
   * https://developercenter.robotstudio.com/api/RWS?urls.primaryName=ControlStation%20Service
   *
   * \return RWSResult containing the write access status information.
   *
   * \throw \a RWSError if something goes wrong.
   */
  RWSClient::RWSResult getWriteAccessStatus() const;

  /**
   * \brief Allow motion control.
   *
   * https://developercenter.robotstudio.com/api/RWS?urls.primaryName=ControlStation%20Service
   *
   * \throw \a RWSError if something goes wrong.
   */
  void allowMotionControl() const;

  /**
   * \brief Release motion control.
   *
   * https://developercenter.robotstudio.com/api/RWS?urls.primaryName=ControlStation%20Service
   *
   * \throw \a RWSError if something goes wrong.
   */
  void releaseMotionControl() const;

 private:
  RWSClient& rws_client_;
};
}  // namespace abb::rws::v2_0::rw::controlstation
