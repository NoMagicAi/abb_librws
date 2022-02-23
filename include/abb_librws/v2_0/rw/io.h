#pragma once

#include <abb_librws/v2_0/rws_client.h>
#include <abb_librws/common/rw/io.h>


namespace abb :: rws :: v2_0 :: rw :: io
{
    using namespace abb::rws::rw::io;


    /**
     * \brief IO signal subscription resource.
     */
    struct IOSignalResource
    {
        /**
         * \brief A constructor.
         *
         * \param name name of the IO signal.
         */
        explicit IOSignalResource(std::string const& name)
        : name(name)
        {
        }

        /**
         * \brief IO signal name.
         */
        std::string name;


        std::string getURI() const;
        void processEvent(Poco::XML::Element const& li_element, SubscriptionCallback& callback) const;
    };
}