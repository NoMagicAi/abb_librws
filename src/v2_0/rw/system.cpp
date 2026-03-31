#include <abb_librws/v2_0/rw/system.h>

#include <abb_librws/parsing.h>
#include <abb_librws/rws_error.h>
#include <abb_librws/v2_0/rws.h>

#include <boost/throw_exception.hpp>

#include <string>


namespace abb :: rws :: v2_0 :: rw :: system
{
    SystemInfo getRobotWareInfo(RWSResult const& rws_result)
    {
        Poco::XML::Node const * li_node = rws_result->getNodeByPath("html/body/div/ul/li");
        if (!li_node)
            BOOST_THROW_EXCEPTION(ProtocolError {"Cannot parse RWS response: can't find XML path html/body/div/ul/li"});

        std::string const major_str = xmlFindTextContent(li_node, XMLAttributes::CLASS_MAJOR);
        if (major_str.empty())
            BOOST_THROW_EXCEPTION(ProtocolError {"Can't find a node with class=\"major\""});
        int major = abb::rws::getIntOrThrow(major_str, "major version number");

        std::string const minor_str = xmlFindTextContent(li_node, XMLAttributes::CLASS_MINOR);
        if (minor_str.empty())
            BOOST_THROW_EXCEPTION(ProtocolError {"Can't find a node with class=\"minor\""});
        int minor = abb::rws::getIntOrThrow(minor_str, "minor version number");

        std::string const revision_str = xmlFindTextContent(li_node, XMLAttributes::CLASS_REVISION);
        if (revision_str.empty())
            BOOST_THROW_EXCEPTION(ProtocolError {"Can't find a node with class=\"revision\""});
        int revision = abb::rws::getIntOrThrow(revision_str, "revision number");

        std::string const sub_revision_str = xmlFindTextContent(li_node, XMLAttributes::CLASS_SUB_REVISION);
        if (sub_revision_str.empty())
            BOOST_THROW_EXCEPTION(ProtocolError {"Can't find a node with class=\"sub_revision\""});
        int sub_revision = abb::rws::getIntOrThrow(sub_revision_str, "sub-revision number");

        std::string const build_str = xmlFindTextContent(li_node, XMLAttributes::CLASS_BUILD);
        if (build_str.empty())
            BOOST_THROW_EXCEPTION(ProtocolError {"Can't find a node with class=\"build\""});
        int build = abb::rws::getIntOrThrow(build_str, "build number");

        std::string const build_tag = xmlFindTextContent(li_node, XMLAttributes::CLASS_BUILD_TAG);

        std::vector<Poco::XML::Node*> node_list = xmlFindNodes(rws_result, XMLAttributes::CLASS_SYS_SYSTEM_LI);
        std::string system_name;
        for (size_t i = 0; i < node_list.size(); ++i)
        {
            system_name = xmlFindTextContent(node_list.at(i), XMLAttributes::CLASS_NAME);
        }

        node_list = xmlFindNodes(rws_result, XMLAttributes::CLASS_SYS_OPTION_LI);
        std::vector<std::string> system_options;
        for (size_t i = 0; i < node_list.size(); ++i)
        {
            system_options.push_back(xmlFindTextContent(node_list.at(i), XMLAttributes::CLASS_OPTION));
        }

        return SystemInfo{
            system_name,
            std::move(system_options),
            RobotWareVersion(major, minor, revision, sub_revision, build, build_tag, system_name)
        };
    }
}
