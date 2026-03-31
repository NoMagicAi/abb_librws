#pragma once

#include <string>


namespace abb :: rws :: rw :: system
{
    RobotWareVersion::RobotWareVersion(int major, int minor, int revision, int subRevision, int build, std::string buildTag){
        this->major = major;
        this->minor = minor;
        this->revision = revision;
        this->subRevision = subRevision;
        this->build = build;
        this->buildTag = buildTag;
    }
}
