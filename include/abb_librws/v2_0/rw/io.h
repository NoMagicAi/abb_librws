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
    struct IOSignalResource
    {
        using Callback = std::function<void (IOSignalStateEvent const&)>;


        /**
         * \brief A constructor.
         *
         * \param name name of the IO signal.
         */
        explicit IOSignalResource(std::string const& name, Callback callback)
        :   name(name)
        ,   callback_ {std::move(callback)}
        {
        }


        std::string getURI() const;
        void processEvent(Poco::XML::Element const& li_element, SubscriptionCallback& callback) const;


    private:
        /**
         * \brief IO signal name.
         */
        std::string name;

        Callback callback_;
    };
}