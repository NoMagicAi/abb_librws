#include <string>

#include <abb_librws/v2_0/rw/motionsystem.h>

namespace abb ::rws ::v2_0 ::rw ::motionsystem {

MotionSystemInterface::MotionSystemInterface(RWSClient& rws_client) : rws_client_(rws_client) {}

void MotionSystemInterface::setMechanicalUnitPosition(std::string const& mechanical_unit_name,
                                                      JointTarget const& position) const {
  std::stringstream uri;
  uri << Resources::RW_MOTIONSYSTEM_MECHUNITS << "/" << mechanical_unit_name << "/position";

  std::string content =
      "rob_joint=" + position.robax.constructString() + "&ext_joint=" + position.extax.constructString();
  std::string content_type = "application/x-www-form-urlencoded;v=2.0";

  rws_client_.httpPost(uri.str(), content, content_type);
}
}  // namespace abb::rws::v2_0::rw::motionsystem
