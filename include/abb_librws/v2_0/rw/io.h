#pragma once

#include <abb_librws/v2_0/rws_client.h>
#include <abb_librws/common/rw/io.h>
#include <abb_librws/rws_subscription.h>

#include <functional>


namespace abb :: rws :: v2_0 :: rw :: io
{
    using namespace abb::rws::rw::io;


    /**
     * \brief IO signal subscription resource.
     */
    struct IOSignalSubscribableResource: SubscribableResource
    {
        /**
         * \brief A constructor.
         *
         * \param name name of the IO signal.
         */
        explicit IOSignalSubscribableResource(std::string const& name)
        :   name(name)
        {
        }


        std::string getURI() const;

    private:
        /**
         * \brief IO signal name.
         */
        std::string name;
    };
}