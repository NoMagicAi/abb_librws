#include <sstream>
#include <string>

#include <Poco/DOM/DOMParser.h>

#include <abb_librws/v2_0/rw/controlstation.h>
#include <abb_librws/v2_0/rws.h>
#include <abb_librws/rws_poco_result.h>

namespace abb ::rws ::v2_0 ::rw ::controlstation {

using abb::rws::v2_0::Resources;
using abb::rws::POCOResult;

ControlStationInterface::ControlStationInterface(RWSClient& rws_client) : rws_client_{rws_client} {}

void ControlStationInterface::registerRemote(std::string const& control_station_name,
                                             std::string const& control_station_id,
                                             std::string const& pincode,
                                             bool release_write_access_when_lost) const {
  std::string content = "control-station-name=" + control_station_name +
                        "&control-station-id=" + control_station_id +
                        "&pincode=" + pincode +
                        "&release-write-access-when-lost=" + (release_write_access_when_lost ? "true" : "false");
  std::string content_type = "application/x-www-form-urlencoded;v=2.0";

  rws_client_.httpPost(Resources::RW_CONTROLSTATION_REGISTER_REMOTE, content, content_type);
}

void ControlStationInterface::requestWriteAccess() const {
  rws_client_.httpPost(Resources::RW_CONTROLSTATION_WRITEACCESS_REQUEST);
}

void ControlStationInterface::releaseWriteAccess() const {
  rws_client_.httpPost(Resources::RW_CONTROLSTATION_WRITEACCESS_RELEASE);
}

RWSClient::RWSResult ControlStationInterface::getWriteAccessStatus() const {
  POCOResult poco_result = rws_client_.httpGet(Resources::RW_CONTROLSTATION_WRITEACCESS_STATUS);

  Poco::XML::DOMParser parser;
  return parser.parseString(poco_result.content());
}

void ControlStationInterface::allowMotionControl() const {
  std::string content = "allow-motion-control=true";
  std::string content_type = "application/x-www-form-urlencoded;v=2.0";
  rws_client_.httpPost(Resources::RW_CONTROLSTATION_MOTIONCONTROL, content, content_type);
}

void ControlStationInterface::releaseMotionControl() const {
  std::string content = "allow-motion-control=false";
  std::string content_type = "application/x-www-form-urlencoded;v=2.0";
  rws_client_.httpPost(Resources::RW_CONTROLSTATION_MOTIONCONTROL, content, content_type);
}

}  // namespace abb::rws::v2_0::rw::controlstation
