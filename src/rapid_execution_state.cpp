#include <abb_librws/rapid_execution_state.h>

#include <boost/throw_exception.hpp>

#include <stdexcept>


namespace abb :: rws
{
    RAPIDExecutionState makeRAPIDExecutionState(std::string const& str)
    {
        if (str == "running")
            return RAPIDExecutionState::running;
        else if (str == "stopped")
            return RAPIDExecutionState::stopped;
        else
            BOOST_THROW_EXCEPTION(std::invalid_argument {"Unexpected string representation of RAPID execution state: \"" + str + "\""});
    }
}