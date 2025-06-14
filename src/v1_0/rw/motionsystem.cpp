#include <sstream>
#include <string>

#include <abb_librws/v1_0/rw/motionsystem.h>

namespace abb ::rws ::v1_0 ::rw ::motionsystem {

MotionSystemInterface::MotionSystemInterface(RWSClient& rws_client) : rws_client_{rws_client} {}

void MotionSystemInterface::setMechanicalUnitPosition(std::string const& mechanical_unit_name,
                                                      JointTarget const& position) const {
  std::stringstream uri;
  uri << Resources::RW_MOTIONSYSTEM_MECHUNITS << "/" << mechanical_unit_name << "?action=mechunit-position";

  std::string content =
      "rob_joint=" + position.robax.constructString() + "&ext_joint=" + position.extax.constructString();

  rws_client_.httpPost(uri.str(), content);
}
}  // namespace abb::rws::v1_0::rw::motionsystem
