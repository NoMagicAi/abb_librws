#include <abb_librws/rws.h>
#include <abb_librws/common/rw/elog.h>
#include <abb_librws/parsing.h>

#include <boost/throw_exception.hpp>
#include <boost/algorithm/string.hpp>

#include <iomanip>
#include <sstream>

namespace abb ::rws ::rw ::elog
{
    std::string to_string(MessageType const messageType)
    {
        switch (messageType)
        {
        case MessageType::INFORMATION:
            return "INFORMATION";
            break;
        case MessageType::WARNING:
            return "WARNING";
            break;
        case MessageType::ERROR:
            return "ERROR";
            break;
        default:
            BOOST_THROW_EXCEPTION(std::logic_error{"Invalid MessageType value"});
        }
    }

    ArgType makeArgType(std::string const &str)
    {
        std::string name = boost::algorithm::to_upper_copy(str);
        if (name == "STRING")
            return ArgType::STRING;
        else if (name == "LONG")
            return ArgType::LONG;
        else if (name == "FLOAT")
            return ArgType::FLOAT;
        std::stringstream msg;
        BOOST_THROW_EXCEPTION(std::invalid_argument("Unexpected string representation of elog message argument type: \"" + str + "\""));
    }

    std::string to_string(ArgType const &argType)
    {
        switch (argType)
        {
        case ArgType::FLOAT:
            return "FLOAT";
            break;
        case ArgType::LONG:
            return "LONG";
            break;
        case ArgType::STRING:
            return "STRING";
            break;
        default:
            BOOST_THROW_EXCEPTION(std::logic_error{"Invalid ArgType value"});
        }
    }

    float ElogMessageArg::toFloat()
    {
        if (argType != ArgType::FLOAT)
        {
            std::stringstream msg;
            msg << "ElogMessageArg type is " << to_string(argType) << " not " << to_string(ArgType::LONG) << "!";
            BOOST_THROW_EXCEPTION(std::invalid_argument(msg.str()));
        }

        return std::stof(value);
    }

    long ElogMessageArg::toLong()
    {
        if (argType != ArgType::LONG)
        {
            std::stringstream msg;
            msg << "ElogMessageArg type is " << to_string(argType) << " not " << to_string(ArgType::LONG) << "!";
            BOOST_THROW_EXCEPTION(std::invalid_argument(msg.str()));
        }
        return std::stol(value);
    }

    const std::string Language::CZECH = "cs";
    const std::string Language::DANISH = "da";
    const std::string Language::GERMAN = "de";
    const std::string Language::ENGLISH = "en";
    const std::string Language::SPANISH = "es";
    const std::string Language::FINNISH = "fi";
    const std::string Language::FRENCH = "fr";
    const std::string Language::HUNGARIAN = "hu";
    const std::string Language::ITALIAN = "it";
    const std::string Language::JAPANESE = "ja";
    const std::string Language::KOREAN = "ko";
    const std::string Language::DUTCH = "nl";
    const std::string Language::POLISH = "pl";
    const std::string Language::PORTUGUESE = "pt";
    const std::string Language::ROMANIAN = "ro";
    const std::string Language::RUSSIAN = "ru";
    const std::string Language::SLOVENIAN = "sl";
    const std::string Language::SWEDISH = "sv";
    const std::string Language::TURKISH = "tr";
    const std::string Language::CHINESE = "zh";

}
