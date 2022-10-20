#include <abb_librws/common/rw/retcode.h>

#include <sstream>

namespace abb::rws::rw::retcode
{
    std::string to_string(RetcodeInfo const& info)
    {
        std::stringstream ss;
        ss << info.severity << " " << info.code << " " << info.name << ": " << info.description;
        return ss.str(); 
    }
}
