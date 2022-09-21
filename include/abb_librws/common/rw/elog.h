#pragma once

#include <string>
#include <vector>
#include <chrono>

namespace abb ::rws ::rw ::elog
{

    /**
     * @brief Types of elog messages
     */
    enum class MessageType
    {
        INFORMATION = 1,
        WARNING = 2,
        ERROR = 3
    };

    std::string to_string(MessageType const messagType);

    /**
     * @brief Types of elog message arguments
     */
    enum class ArgType
    {
        STRING,
        LONG,
        FLOAT
    };

    ArgType makeArgType(std::string const &str);

    std::string to_string(ArgType const &argType);

    /**
     * @brief Argument of elog message
     * https://developercenter.robotstudio.com/api/rwsApi/elog_domain_get_page.html
     */
    struct ElogMessageArg
    {
        int argNum;
        ArgType argType;
        std::string value;

        float toFloat();
        long toLong();
    };

    /**
     * @brief Elog message structure
     * https://developercenter.robotstudio.com/api/rwsApi/elog_domain_get_page.html
     */
    struct ElogMessage
    {
        int domain;                       // Domain of message.
        int sequenceNumber;               // Sequence number of message in domain.
        MessageType messageType;          // The elog message type.
        int code;                         // The elog message code.
        std::string sourceName;           // The elog message source.
        std::tm timestamp;                // The time stamp when the event log was generated.
        std::string title;                // The elog message title in the specified langauge. Available only when lang parameter is provided.
        std::string desc;                 // The elog message description in the specified langauge. Available only when lang parameter is provided.
        std::string conseqs;              // The elog message consequences in the specified langauge. Available only when lang parameter is provided.
        std::string causes;               // The elog message causes in the specified langauge. Available only when lang parameter is provided.
        std::string actions;              // The elog message actions in the specified langauge. Available only when lang parameter is provided.
        std::vector<ElogMessageArg> argv; // The argument's position e.g. arg1, arg2 etc.. The type of argument can be float, string or long.
    };

    struct Language final
    {
        static const std::string CZECH;
        static const std::string DANISH;
        static const std::string GERMAN;
        static const std::string ENGLISH;
        static const std::string SPANISH;
        static const std::string FINNISH;
        static const std::string FRENCH;
        static const std::string HUNGARIAN;
        static const std::string ITALIAN;
        static const std::string JAPANESE;
        static const std::string KOREAN;
        static const std::string DUTCH;
        static const std::string POLISH;
        static const std::string PORTUGUESE;
        static const std::string ROMANIAN;
        static const std::string RUSSIAN;
        static const std::string SLOVENIAN;
        static const std::string SWEDISH;
        static const std::string TURKISH;
        static const std::string CHINESE;
    };
}
