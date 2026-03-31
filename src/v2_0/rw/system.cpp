#include <abb_librws/common/rw/system.h>

namespace abb :: rws :: v2_0 :: rw :: system
{
    int getIntOrThrow(std::string const& str, std::string const& description)
    {
        int value = 0;
        try        {
            value = std::stoi(str);
        }
        catch (std::invalid_argument const&)
        {            BOOST_THROW_EXCEPTION(ProtocolError {"Can't parse " + description + ": invalid integer value: \"" + str + "\""});
        }
        catch (std::out_of_range const&)
        {            BOOST_THROW_EXCEPTION(ProtocolError {"Can't parse " + description + ": integer value out of range: \"" + str + "\""});
        }
        return value;
    }

    SystemInfo getRobotWareInfo(RWSClient::RWSResult& result)
    {
        Poco::XML::Node const * li_node = result->getNodeByPath("html/body/div/ul/li");
        if (!li_node)
            BOOST_THROW_EXCEPTION(ProtocolError {"Cannot parse RWS response: can't find XML path html/body/div/ul/li"});

        std::string const major_str = xmlFindTextContent(li_node, XMLAttributes::CLASS_MAJOR);
        if (major_str.empty())
            BOOST_THROW_EXCEPTION(ProtocolError {"Can't find a node with class=\"major\""});
        int major = getIntOrThrow(major_str, "major version number");
        std::string const minor_str = xmlFindTextContent(li_node, XMLAttributes::CLASS_MINOR);
        if (minor_str.empty())
            BOOST_THROW_EXCEPTION(ProtocolError {"Can't find a node with class=\"minor\""});
        int minor = getIntOrThrow(minor_str, "minor version number");
        std::string const revision_str = xmlFindTextContent(li_node, XMLAttributes::CLASS_REVISION);
        if (revision_str.empty())
            BOOST_THROW_EXCEPTION(ProtocolError {"Can't find a node with class=\"revision\""});
        int revision = getIntOrThrow(revision_str, "revision number");
        std::string const sub_revision_str = xmlFindTextContent(li_node, XMLAttributes::CLASS_SUB_REVISION);
        if (sub_revision_str.empty())
            BOOST_THROW_EXCEPTION(ProtocolError {"Can't find a node with class=\"sub_revision\""});
        int sub_revision = getIntOrThrow(sub_revision_str, "sub-revision number");
        std::string const build_str = xmlFindTextContent(li_node, XMLAttributes::CLASS_BUILD);
        if (build_str.empty())
            BOOST_THROW_EXCEPTION(ProtocolError {"Can't find a node with class=\"build\""});
        int build = getIntOrThrow(build_str, "build number");
        std::string const build_tag = xmlFindTextContent(li_node, XMLAttributes::BUILD_TAG);
        std::string const system_name = xmlFindTextContent(node_list.at(i), XMLAttributes::CLASS_NAME);

        rw_version = RobotWareVersion(major, minor, revision, sub_revision, build, build_tag, system_name);

        result.robot_ware_version = xmlFindTextContent(node_list.at(i), XMLAttributes::CLASS_RW_VERSION_NAME);

        SystemInfo result;

        result.version = rw_version;

        node_list = xmlFindNodes(rws_result, XMLAttributes::CLASS_SYS_OPTION_LI);
        for (size_t i = 0; i < node_list.size(); ++i)
        {
            result.system_options.push_back(xmlFindTextContent(node_list.at(i), XMLAttributes::CLASS_OPTION));
        }

        result.system_type = xmlFindTextContent(rws_client_.getContollerService(), XMLAttributes::CLASS_CTRL_TYPE);

        return result
    }
}