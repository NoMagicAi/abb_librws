#pragma once

#include <abb_librws/v1_0/rws_client.h>

#include <Poco/DOM/DOMParser.h>


namespace abb :: rws :: v1_0 :: rw
{
    class RAPIDService
    {
    public:
        explicit RAPIDService(RWSClient& client);

        /**
         * \brief A method for retrieving the data of a RAPID symbol.
         *
         * \param resource specifying the RAPID task, module and symbol names for the RAPID resource.
         *
         * \return RWSResult containing the result.
         *
         * \throw \a RWSError if something goes wrong.
         */
        void getRAPIDSymbolData(RAPIDResource const& resource, RAPIDSymbolDataAbstract& data);

        /**
         * \brief A method for retrieving the data of a RAPID symbol in raw text format.
         *
         * See the corresponding "setRAPIDSymbolData(...)" method for examples of RAPID symbols in raw text format.
         *
         * \param task name of the RAPID task containing the RAPID symbol.
         * \param module name of the RAPID module containing the RAPID symbol.
         * \param name name of the RAPID symbol.
         *
         * \return String containing the data.
         *
         * \throw \a RWSError if something goes wrong.
         */
        std::string getRAPIDSymbolData(RAPIDResource const& resource);

        /**
         * \brief A method for setting the data of a RAPID symbol (based on the provided struct representing the RAPID data).
         *
         * \param resource specifying the RAPID task, module and symbol names for the RAPID resource.
         * \param data for the RAPID symbol's new data.
         *
         * \throw \a RWSError if something goes wrong.
         */
        void setRAPIDSymbolData(const RAPIDResource& resource, const RAPIDSymbolDataAbstract& data);

        /**
         * \brief A method for setting the data of a RAPID symbol.
         *
         * \param resource specifying the RAPID task, module and symbol names for the RAPID resource.
         * \param data the RAPID symbol's new data.
         *
         * \throw \a RWSError if something goes wrong.
         */
        void setRAPIDSymbolData(const RAPIDResource& resource, const std::string& data);

    private:
        /**
         * \brief A method for retrieving the properties of a RAPID symbol.
         *
         * \param resource specifying the RAPID task, module and symbol names for the RAPID resource.
         *
         * \return RWSResult containing the result.
         *
         * \throw \a RWSError if something goes wrong.
         */
        RWSResult getRAPIDSymbolProperties(RAPIDResource const& resource);

        /**
         * \brief Method for generating a RAPID data resource URI path.
         *
         * \param resource specifying the RAPID task, module and symbol names for the RAPID resource.
         *
         * \return std::string containing the path.
         */
        static std::string generateRAPIDDataPath(const RAPIDResource& resource);

        /**
         * \brief Method for generating a RAPID properties resource URI path.
         *
         * \param resource specifying the RAPID task, module and symbol names for the RAPID resource.
         *
         * \return std::string containing the path.
         */
        static std::string generateRAPIDPropertiesPath(const RAPIDResource& resource);

        RWSClient& client_;
        Poco::XML::DOMParser parser_;
    };
}