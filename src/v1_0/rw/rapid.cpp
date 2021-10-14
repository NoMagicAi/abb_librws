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
}