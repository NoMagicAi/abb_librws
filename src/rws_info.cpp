#pragma once

#include <string>
#include <abb_librws/rws_info.h>


namespace abb :: rws :: rw :: system
{
    RobotWareVersion::RobotWareVersion(int major, int minor, int revision, int sub_revision, int build, std::string build_tag, std::string robot_ware_version_name){
        this->major = major;
        this->minor = minor;
        this->revision = revision;
        this->sub_revision = sub_revision;
        this->build = build;
        this->build_tag = build_tag;
        this->robot_ware_version_name = robot_ware_version_name;
    }
}
