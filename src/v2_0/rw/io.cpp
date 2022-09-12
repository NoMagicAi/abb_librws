#include <abb_librws/v2_0/rw/io.h>
#include <abb_librws/parsing.h>

namespace abb :: rws :: v2_0 :: rw :: io
{
    std::string IOSignalSubscribableResource::getURI() const
    {
      std::string resource_uri = Resources::RW_IOSYSTEM_SIGNALS;
      resource_uri += "/";
      resource_uri += name;
      resource_uri += ";";
      resource_uri += Identifiers::STATE;
      return resource_uri;
    }
}