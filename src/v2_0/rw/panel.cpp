#include <abb_librws/v2_0/rw/panel.h>
#include <abb_librws/v2_0/rws.h>

#include <abb_librws/parsing.h>
#include <abb_librws/system_constants.h>


namespace abb :: rws :: v2_0 :: rw :: panel
{
    ControllerState getControllerState(RWSClient& client)
    {
        std::string uri = Resources::RW_PANEL_CTRLSTATE;
        RWSResult xml_content = parseXml(client.httpGet(uri).content());

        Poco::XML::Node const * li_node = xml_content->getNodeByPath("html/body/div/ul/li");
        if (!li_node)
            BOOST_THROW_EXCEPTION(ProtocolError {"Cannot parse RWS response: can't find XML path html/body/div/ul/li"});

        std::string const ctrlstate = xmlFindTextContent(li_node, XMLAttributes::CLASS_CTRLSTATE);
        if (ctrlstate.empty())
            BOOST_THROW_EXCEPTION(ProtocolError {"Can't find a node with class=\"ctrlstate\""});

        return makeControllerState(ctrlstate);
    }


    OperationMode getOperationMode(RWSClient& client)
    {
        std::string uri = Resources::RW_PANEL_OPMODE;
        RWSResult xml_content = parseXml(client.httpGet(uri).content());

        Poco::XML::Node const * li_node = xml_content->getNodeByPath("html/body/div/ul/li");
        if (!li_node)
            BOOST_THROW_EXCEPTION(ProtocolError {"Cannot parse RWS response: can't find XML path html/body/div/ul/li"});

        std::string const opmode = xmlFindTextContent(li_node, XMLAttributes::CLASS_OPMODE);
        if (opmode.empty())
            BOOST_THROW_EXCEPTION(ProtocolError {"Can't find a node with class=\"opmode\""});

        return makeOperationMode(opmode);
    }


    void setControllerState(RWSClient& client, ControllerState state)
    {
        std::string uri = Resources::RW_PANEL_CTRLSTATE;
        std::stringstream content;
        content << "ctrl-state=" << state;
        std::string content_type = "application/x-www-form-urlencoded;v=2.0";

        client.httpPost(uri, content.str(), content_type);
    }


    unsigned getSpeedRatio(RWSClient& client)
    {
        std::string uri = "/rw/panel/speedratio";
        RWSResult rws_result = parseXml(client.httpGet(uri).content());

        return std::stoul(xmlFindTextContent(rws_result, XMLAttribute(Identifiers::CLASS, "speedratio")));
    }


    void setSpeedRatio(RWSClient& client, unsigned int ratio)
    {
        if (ratio > 100)
            BOOST_THROW_EXCEPTION(std::out_of_range {"Speed ratio argument out of range (should be 0 <= ratio <= 100)"});

        std::string uri = "/rw/panel/speedratio?action=setspeedratio";
        std::stringstream content;
        content << "speed-ratio=" << ratio;

        client.httpPost(uri, content.str());
    }

    std::string ControllerStateSubscribableResource::getURI() const
    {
      return "/rw/panel/ctrl-state";
    }


    std::string OperationModeSubscribableResource::getURI() const
    {
      return "/rw/panel/opmode";
    }


    bool ControllerStateSubscribableResource::canProcessEvent(Poco::XML::Element const& li_element) const
    {
        return li_element.getAttribute("class") == "pnl-ctrlstate-ev";
    }


    bool OperationModeSubscribableResource::canProcessEvent(Poco::XML::Element const& li_element) const
    {
        return li_element.getAttribute("class") == "pnl-opmode-ev";
    }


    void ControllerStateSubscribableResource::processEvent(Poco::XML::Element const& li_element, SubscriptionCallback& callback) const
    {
        ControllerStateEvent event;
        event.state = rw::makeControllerState(xmlFindTextContent(&li_element, XMLAttribute {"class", "ctrlstate"}));
        event.resource = std::make_shared<ControllerStateSubscribableResource>();

        callback.processEvent(event);
    }


    void OperationModeSubscribableResource::processEvent(Poco::XML::Element const& li_element, SubscriptionCallback& callback) const
    {
        OperationModeEvent event;
        event.mode = rw::makeOperationMode(xmlFindTextContent(&li_element, XMLAttribute {"class", "opmode"}));
        event.resource = std::make_shared<OperationModeSubscribableResource>();

        callback.processEvent(event);
    }
}