#include <abb_librws/rws_info.h>

#include <string>


namespace abb :: rws
{
    RobotWareVersion::RobotWareVersion(int major, int minor, int revision, int sub_revision, int build, std::string build_tag, std::string robot_ware_version_name)
        : major(major)
        , minor(minor)
        , revision(revision)
        , sub_revision(sub_revision)
        , build(build)
        , build_tag(std::move(build_tag))
        , robot_ware_version_name(std::move(robot_ware_version_name))
    {
    }
}
