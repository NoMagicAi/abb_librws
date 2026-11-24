#pragma once

#include <abb_librws/rws.h>
#include <abb_librws/rws_resource.h>
#include <abb_librws/v2_0/rws_client.h>
#include <abb_librws/common/rw/elog.h>

namespace abb ::rws ::v2_0 ::rw ::elog
{
    using namespace rws::rw::elog;

    /**
     * \brief Event log subscription resource
     */
    struct ElogSubscribableResource : public SubscribableResource
    {
        explicit ElogSubscribableResource(int domain);

        [[nodiscard]] std::string getURI() const override;

        void processEvent(Poco::XML::Element const &li_element, std::function<void(SubscriptionEvent const&)> const& callback) const override;

    private:
        int const domain_;
    };

    /**
     * @brief Get a list of all elog domains.
     * https://developercenter.robotstudio.com/api/rwsApi/elog_resources_page.html
     *
     * @param client RWSClient
     * @param lang The language in which domain names will be returned. English default.
     * @return std::vector<std::pair<int, std::string>> - List of domain id and domain name in provided language
     */
    std::vector<std::pair<int, std::string>> getElogDomains(RWSClient &client, std::string lang = Language::ENGLISH);

    /**
     * @brief Get a specific elog message in the given domain.
     * https://developercenter.robotstudio.com/api/rwsApi/elog_domain_get_page.html
     *
     * @param client RWSClient
     * @param domain domain number
     * @param seqnum sequence number to start from. If -1, the last @param limit messages are returned. Default -1.
     * @param lang The language in which messages will be returned. English default.
     *             Empty: no lang parameter, so the messages have no text (title, desc, conseqs, causes, actions),
     *             only the code and the arguments. The controller then does no text work, answers faster
     *             and returns up to 200 messages (with lang at most 50).
     * @param limit The number of messages to return. Default 50.
     * @param order The order in which messages will be returned (fifo or lifo). LIFO default.
     * @return ElogMessage structure
     */
    std::vector<ElogMessage> getElogMessages(RWSClient& client, int const domain, int const seqnum = -1, std::string const& lang = Language::ENGLISH, int const limit = 50, Order const order = Order::LIFO);

    /**
     * @brief Get the elog text registry of the controller (GET /ctrl/registry/elogtext).
     *
     * The registry lists the elog text file of each domain. The path of a file in language xx is
     * prepath + xx + postpath; it can be read with the file service. Not in the public RWS documentation,
     * used by the ABB PC SDK. Available on RobotWare 7 real and virtual controllers.
     *
     * @param client RWSClient
     * @return The registry XML document (namespace urn:abb-robotics-registry-elog-text).
     */
    std::string getElogTextRegistry(RWSClient& client);

    /**
     * @brief Extract the elog text registry XML document from the reply of GET /ctrl/registry/elogtext.
     *
     * @throw std::invalid_argument if the reply has no registry.
     */
    std::string parseElogTextRegistry(std::string const& xml_string);
}
