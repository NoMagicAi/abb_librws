#pragma once

#include <string>


namespace abb :: rws
{
    /**
     * \brief State of RAPID program.
     */
    enum class RAPIDExecutionState : bool
    {
        stopped = false,
        running = true
    };


    /**
     * \brief Create \a RAPIDExecutionState from string.
     *
     * \param str source string
     *
     * \return \a RAPIDExecutionState matching the value of \a str
     *
     * \throw \a std::invalid_argument if \a str is not from the set of valid strings.
     */
    RAPIDExecutionState makeRAPIDExecutionState(std::string const& str);
}