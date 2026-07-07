#include <sstream>
#include <string>

#include <Poco/DOM/DOMParser.h>
#include <Poco/DOM/NodeList.h>
#include <Poco/DOM/Element.h>

#include <abb_librws/v2_0/rw/controlstation.h>
#include <abb_librws/v2_0/rws.h>
#include <abb_librws/rws_poco_result.h>
#include <abb_librws/parsing.h>
#include <abb_librws/xml_attribute.h>

namespace abb ::rws ::v2_0 ::rw ::controlstation {

using abb::rws::v2_0::Resources;
using abb::rws::POCOResult;

namespace {
constexpr char const* FORM_URLENCODED_CONTENT_TYPE = "application/x-www-form-urlencoded;v=2.0";
}  // namespace

ControlStationInterface::ControlStationInterface(RWSClient& rws_client) : rws_client_{rws_client} {}

void ControlStationInterface::registerRemote(std::string const& control_station_name,
                                             std::string const& control_station_id,
                                             std::string const& pincode,
                                             bool release_write_access_when_lost) const {
  std::string content = "control-station-name=" + control_station_name +
                        "&control-station-id=" + control_station_id +
                        "&pincode=" + pincode +
                        "&release-write-access-when-lost=" + (release_write_access_when_lost ? "true" : "false");

  rws_client_.httpPost(Resources::RW_CONTROLSTATION_REGISTER_REMOTE, content, FORM_URLENCODED_CONTENT_TYPE);
}

void ControlStationInterface::requestWriteAccess() const {
  rws_client_.httpPost(Resources::RW_CONTROLSTATION_WRITEACCESS_REQUEST,"",FORM_URLENCODED_CONTENT_TYPE);
}

void ControlStationInterface::releaseWriteAccess() const {
  rws_client_.httpPost(Resources::RW_CONTROLSTATION_WRITEACCESS_RELEASE,"",FORM_URLENCODED_CONTENT_TYPE);
}

WriteAccessStatus WriteAccessStatus::parse(Poco::XML::Element const* li_element) {
  if (!li_element) {
    throw std::runtime_error("Failed to find controlstation-write-access-status element");
  }

  WriteAccessStatus status;

  // Parse all fields from the XML element
  // Expected XML structure:
  // <li class="controlstation-write-access-status">
  //   <span class="held-by-control-station-Id">none</span>
  //   <span class="held-by-control-station-name">none</span>
  //   <span class="control-station-write-access-held">false</span>
  //   <span class="control-station-external-control-enabled">true</span>
  // </li>

  status.held_by_control_station_id = xmlFindTextContent(li_element, XMLAttribute("class", "held-by-control-station-Id"));
  status.held_by_control_station_name = xmlFindTextContent(li_element, XMLAttribute("class", "held-by-control-station-name"));

  std::string write_access_held_str = xmlFindTextContent(li_element, XMLAttribute("class", "control-station-write-access-held"));
  status.control_station_write_access_held = (write_access_held_str == "true");

  std::string external_control_enabled_str = xmlFindTextContent(li_element, XMLAttribute("class", "control-station-external-control-enabled"));
  status.control_station_external_control_enabled = (external_control_enabled_str == "true");

  return status;
}

WriteAccessStatus ControlStationInterface::getWriteAccessStatus() const {
  POCOResult poco_result = rws_client_.httpGet(Resources::RW_CONTROLSTATION_WRITEACCESS_STATUS);

  Poco::XML::DOMParser parser;
  RWSClient::RWSResult rws_result = parser.parseString(poco_result.content());

  Poco::XML::Element const* li_element =
    dynamic_cast<Poco::XML::Element const*>(
      rws_result->getNodeByPath("html/body/div/ul/li[@class='controlstation-write-access-status']"));

  return WriteAccessStatus::parse(li_element);
}

void ControlStationInterface::allowMotionControl() const {
  std::string content = "allow-motion-control=true";
  rws_client_.httpPost(Resources::RW_CONTROLSTATION_MOTIONCONTROL, content, FORM_URLENCODED_CONTENT_TYPE);
}

void ControlStationInterface::releaseMotionControl() const {
  std::string content = "allow-motion-control=false";
  rws_client_.httpPost(Resources::RW_CONTROLSTATION_MOTIONCONTROL, content, FORM_URLENCODED_CONTENT_TYPE);
}

std::string ControlStationWriteAccessStatusSubscribableResource::getURI() const {
  return "/rw/controlstation/writeaccess/status";
}

void ControlStationWriteAccessStatusSubscribableResource::processEvent(Poco::XML::Element const& li_element, std::function<void(SubscriptionEvent const&)> const& callback) const {
  // Check if this is a control station write access status event
  if (li_element.getAttribute("class") == "controlstation-ev") {
    ControlStationExternalControlEvent event;

    // Parse subscription event format (different from GET request format)
    auto status = std::make_shared<WriteAccessStatus>();

    status->held_by_control_station_id = xmlFindTextContent(&li_element, XMLAttribute("class", "control-station-Id"));
    status->held_by_control_station_name = xmlFindTextContent(&li_element, XMLAttribute("class", "control-station-name"));

    std::string write_access_held_str = xmlFindTextContent(&li_element, XMLAttribute("class", "write-access-held"));
    status->control_station_write_access_held = (write_access_held_str == "true");

    std::string external_control_enabled_str = xmlFindTextContent(&li_element, XMLAttribute("class", "external-control-Enabled"));
    status->control_station_external_control_enabled = (external_control_enabled_str == "true");

    event.status = status;
    event.resource = std::make_shared<ControlStationWriteAccessStatusSubscribableResource>();

    callback(event);
  }
}

}  // namespace abb::rws::v2_0::rw::controlstation
