#pragma once

#include <string>


namespace abb :: rws :: rw :: system
{
    /**
     * \brief Robot controller firmware version
     */
    struct RobotWareVersion
    {
        explicit RobotWareVersion(int major, int minor, int revision, int subRevision, int build, std::string buildTag);

        const int major;
        const int minor;
        const int revision;
        const int subRevision;
        const int build;
        const std::string buildTag;
    };
}
