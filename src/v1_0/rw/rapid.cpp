#include <abb_librws/v1_0/rw/rapid.h>

#include <abb_librws/parsing.h>


namespace abb :: rws :: v1_0 :: rw
{
    RAPIDService::RAPIDService(RWSClient& client)
    :   client_ {client}
    {
    }


    void RAPIDService::getRAPIDSymbolData(RAPIDResource const& resource, RAPIDSymbolDataAbstract& data)
    {
        RWSResult const temp_result = getRAPIDSymbolProperties(resource);
        std::string const data_type = xmlFindTextContent(temp_result, XMLAttributes::CLASS_DATTYP);

        if (data.getType() == data_type)
            data.parseString(getRAPIDSymbolData(resource));
        else
            BOOST_THROW_EXCEPTION(std::invalid_argument {"Argument type does not match the RAPID variable type"});
    }


    void RAPIDService::setRAPIDSymbolData(const RAPIDResource& resource, const RAPIDSymbolDataAbstract& data)
    {
        setRAPIDSymbolData(resource, data.constructString());
    }


    std::string RAPIDService::getRAPIDSymbolData(RAPIDResource const& resource)
    {
        std::string const uri = generateRAPIDDataPath(resource);
        RWSResult xml_content = parser_.parseString(client_.httpGet(uri).content());
        std::string value = xmlFindTextContent(xml_content, XMLAttributes::CLASS_VALUE);

        if (value.empty())
            BOOST_THROW_EXCEPTION(std::logic_error {"RAPID value string was empty"});

        return value;
    }


    void RAPIDService::setRAPIDSymbolData(const RAPIDResource& resource, const std::string& data)
    {
        std::string uri = generateRAPIDDataPath(resource) + "?" + Queries::ACTION_SET;
        std::string content = Identifiers::VALUE + "=" + data;

        client_.httpPost(uri, content);
    }


    RAPIDExecutionInfo RAPIDService::getRAPIDExecution()
    {
        RAPIDExecutionInfo result;

        std::string const uri = Resources::RW_RAPID_EXECUTION;
        RWSResult xml_content = parser_.parseString(client_.httpGet(uri).content());

        Poco::XML::Node const * li_node = xml_content->getNodeByPath("html/body/div/ul/li");
        if (!li_node)
            BOOST_THROW_EXCEPTION(ProtocolError {"Cannot parse RWS response: can't find XML path html/body/div/ul/li"});

        std::string const ctrlexecstate = xmlFindTextContent(li_node, XMLAttributes::CLASS_CTRLEXECSTATE);
        if (ctrlexecstate.empty())
            BOOST_THROW_EXCEPTION(ProtocolError {"Can't find a node with class=\"ctrlexecstate\""});

        std::string const cycle = xmlFindTextContent(li_node, XMLAttribute {"class", "cycle"});
        if (cycle.empty())
            BOOST_THROW_EXCEPTION(ProtocolError {"Can't find a node with class=\"cycle\""});

        return RAPIDExecutionInfo {
            makeRAPIDExecutionState(ctrlexecstate),
            makeRAPIDRunMode(cycle)
        };
    }


    RWSResult RAPIDService::getRAPIDSymbolProperties(RAPIDResource const& resource)
    {
        std::string const uri = generateRAPIDPropertiesPath(resource);
        return parser_.parseString(client_.httpGet(uri).content());
    }


    std::string RAPIDService::generateRAPIDDataPath(const RAPIDResource& resource)
    {
        return "/rw/rapid/symbol/data/RAPID/" + resource.task + "/" + resource.module + "/" + resource.name;
    }


    std::string RAPIDService::generateRAPIDPropertiesPath(const RAPIDResource& resource)
    {
        return "/rw/rapid/symbol/properties/RAPID/" + resource.task + "/" + resource.module + "/"+ resource.name;
    }


    RAPIDRunMode makeRAPIDRunMode(std::string const& str)
    {
        if (str == "forever")
            return RAPIDRunMode::forever;
        else if (str == "asis")
            return RAPIDRunMode::asis;
        else if (str == "once")
            return RAPIDRunMode::once;
        else if (str == "oncedone")
            return RAPIDRunMode::oncedone;
        else
            BOOST_THROW_EXCEPTION(std::invalid_argument {"Unexpected string representation of RAPID run mode: \"" + str + "\""});
    }
}