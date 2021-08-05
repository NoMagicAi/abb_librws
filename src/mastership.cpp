#include <abb_librws/mastership.h>

#include <boost/exception/diagnostic_information.hpp>

#include <iostream>


namespace abb :: rws
{
    Mastership::Mastership(MastershipManager& mastership_manager)
    :   mastership_manager_ {mastership_manager}
    {
        try
        {
            mastership_manager_.requestMastership();
        }
        catch (const boost::exception& e)
        {
            std::cout << boost::diagnostic_information(e) << std::endl;
            throw;
        }
    }

    Mastership::~Mastership()
    {
        try
        {
            mastership_manager_.releaseMastership();
        }
        catch (...)
        {
            std::cerr << boost::current_exception_diagnostic_information() << std::endl;
        }
    }
}
