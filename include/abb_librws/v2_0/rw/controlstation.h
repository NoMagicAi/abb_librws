#pragma once
#include <string>

#include <abb_librws/v2_0/rws_client.h>
#include <abb_librws/rws.h>

namespace abb ::rws ::v2_0 ::rw ::controlstation {

/**
 * \brief Write access status information from the control station.
 */
struct WriteAccessStatus {
  std::string held_by_control_station_id;
  std::string held_by_control_station_name;
  bool control_station_write_access_held;
  bool control_station_external_control_enabled;

  /**
   * \brief Parse WriteAccessStatus from a Poco XML Element.
   *
   * \param li_element XML element containing the control station write access status data
   * \return WriteAccessStatus parsed from the XML element
   *
   * \throw std::runtime_error if element is null
   */
  static WriteAccessStatus parse(Poco::XML::Element const* li_element);
};

/**
 * \brief Control station write access status subscription resource
 */
struct ControlStationWriteAccessStatusSubscribableResource: public SubscribableResource
{
  [[nodiscard]] std::string getURI() const override;

  void processEvent(Poco::XML::Element const& li_element, std::function<void(SubscriptionEvent const&)> const& callback) const override;
};

/**
 * \brief A class for operating on controlstation in ABB controller (RobotWare >= 8).
 *
 * https://robotwebservices.robotics.abb.com/api/service?stringValue=Control_Station_Service&resourceVersion=rw8
 */
class ControlStationInterface {
 public:
  explicit ControlStationInterface(RWSClient& rws_client);

  /**
   * \brief Register as remote control station.
   *
   * https://robotwebservices.robotics.abb.com/api/service?stringValue=Control_Station_Service&resourceVersion=rw8
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
   * https://robotwebservices.robotics.abb.com/api/service?stringValue=Control_Station_Service&resourceVersion=rw8
   *
   * \throw \a RWSError if something goes wrong.
   */
  void requestWriteAccess() const;

  /**
   * \brief Release the write access currently held by this control station.
   *
   * https://robotwebservices.robotics.abb.com/api/service?stringValue=Control_Station_Service&resourceVersion=rw8
   *
   * \throw \a RWSError if something goes wrong.
   */
  void releaseWriteAccess() const;

  /**
   * \brief Check the current write access status.
   *
   * https://robotwebservices.robotics.abb.com/api/service?stringValue=Control_Station_Service&resourceVersion=rw8
   *
   * \return WriteAccessStatus containing the parsed write access status information.
   *
   * \throw \a RWSError if something goes wrong.
   */
  WriteAccessStatus getWriteAccessStatus() const;

  /**
   * \brief Allow motion control.
   *
   * https://robotwebservices.robotics.abb.com/api/service?stringValue=Control_Station_Service&resourceVersion=rw8
   *
   * \throw \a RWSError if something goes wrong.
   */
  void allowMotionControl() const;

  /**
   * \brief Release motion control.
   *
   * https://robotwebservices.robotics.abb.com/api/service?stringValue=Control_Station_Service&resourceVersion=rw8
   *
   * \throw \a RWSError if something goes wrong.
   */
  void releaseMotionControl() const;

 private:
  RWSClient& rws_client_;
};
}  // namespace abb::rws::v2_0::rw::controlstation
