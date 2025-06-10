#pragma once
#include <string>

#include <abb_librws/rws_rapid.h>
#include <abb_librws/v1_0/rws_client.h>

namespace abb ::rws ::v1_0 ::rw ::motionsystem {
/**
 * \brief A class for operating on motionsystem in ABB controller.
 *
 * https://developercenter.robotstudio.com/api/RWS?urls.primaryName=MotionSystem%20Service
 */
class MotionSystemInterface {
 public:
  explicit MotionSystemInterface(RWSClient& rws_client);
  /**
   * \brief Sets the mechanical unit joint position. Works only on a virtual mechanical unit.
   *
   * https://developercenter.robotstudio.com/api/RWS?urls.primaryName=MotionSystem%20Service
   *
   * \param mechanical_unit_name name of the mechanical unit to set the position for.
   * \param position JointTarget containing the joint position to set.
   *
   * \throw \a RWSError if something goes wrong.
   */
  void setMechanicalUnitPosition(std::string const& mechanical_unit_name, JointTarget const& position) const;

 private:
  RWSClient& rws_client_;
};
}  // namespace abb::rws::v1_0::rw::motionsystem