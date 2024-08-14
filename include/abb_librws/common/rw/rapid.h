#pragma once

#include <abb_librws/rws.h>

#include <string>
#include <iosfwd>
#include <utility>


namespace abb :: rws :: rw
{
    /**
     * \brief run mode of a RAPID program.
     */
    enum class RAPIDRunMode
    {
        forever,
        asis,
        once,
        oncedone
    };


    /**
     * \brief Create \a RAPIDRunMode from string.
     *
     * \param str source string
     *
     * \return \a RAPIDRunMode matching the value of \a str
     *
     * \throw \a std::invalid_argument if \a str is not from the set of valid strings.
     */
    RAPIDRunMode makeRAPIDRunMode(std::string const& str);


    /**
     * \brief State of RAPID program.
     */
    enum class RAPIDExecutionState : bool
    {
        stopped = false,
        running = true
    };


    std::ostream& operator<<(std::ostream& os, RAPIDExecutionState const& state);


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


    /**
     * \brief Execution state and run mode of a RAPID program.
     */
    struct RAPIDExecutionInfo
    {
        /// \brief Rapid execution state
        RAPIDExecutionState ctrlexecstate;

        /// Current run mode
        RAPIDRunMode cycle;
    };


    /**
     * \brief A struct for containing information about a RAPID module.
     */
    struct RAPIDModuleInfo
    {
        /**
         * \brief A constructor.
         *
         * \param name for the name of the module.
         * \param type for the type of the module.
         */
        RAPIDModuleInfo(const std::string& name, const std::string& type)
        :
        name(name),
        type(type)
        {}

        /**
         * \brief The module's name.
         */
        std::string name;

        /**
         * \brief The module's type.
         */
        std::string type;
    };


    /**
     * \brief Execution state of a RAPID task.
     */
    enum class RAPIDTaskExecutionState
    {
        UNKNOWN,      ///< The task state is unknown.
        READY,        ///< The task is ready.
        STOPPED,      ///< The task has been stopped.
        STARTED,      ///< The task has been started.
        UNINITIALIZED ///< The task has not been initialized.
    };


    /**
     * \brief A struct for containing information about a RAPID task.
     */
    struct RAPIDTaskInfo
    {
        /**
         * \brief A constructor.
         *
         * \param name for the name of the task.
         * \param is_motion_task indicating if the task is a motion task or not.
         * \param is_active indicating if the task is active or not.
         * \param execution_state indicating the task's current execution state.
         */
        RAPIDTaskInfo(const std::string& name,
                    const bool is_motion_task,
                    const bool is_active,
                    const RAPIDTaskExecutionState execution_state)
        :
        name(name),
        is_motion_task(is_motion_task),
        is_active(is_active),
        execution_state(execution_state)
        {}

        /**
         * \brief The task's name.
         */
        std::string name;

        /**
         * \brief Flag indicating if the task is a motion task.
         */
        bool is_motion_task;

        /**
         * \brief Flag indicating if the task is active or not.
         */
        bool is_active;

        /**
         * \brief The current execution state of the task.
         */
        RAPIDTaskExecutionState execution_state;
    };

    /**
     * \brief A struct for containing information about a RAPID task pointer is.
     */
    struct RAPIDPcpInfo
    {
        /**
           * \brief A constructor.
           *
           * \param begin_position position where the pointer position starts.
           * \param end_position position where the pointer position ends.
           * \param module_name the name of the module where pointer is.
           * \param routine_name the name of the routine where pointer is.
         */
        RAPIDPcpInfo(std::string  begin_position,
                     std::string  end_position,
                     std::string  module_name,
                     std::string  routine_name)
        :
        begin_position(std::move(begin_position)),
        end_position(std::move(end_position)),
        module_name(std::move(module_name)),
        routine_name(std::move(routine_name))
        {}

        /**
           * \brief Where the pointer position starts.
         */
        std::string begin_position;
        /**
           * \brief Where the pointer position ends.
         */
        std::string end_position;
        /**
           * \brief The name of the module where pointer is.
         */
        std::string module_name;
        /**
           * \brief The name of the routine where pointer is.
         */
        std::string routine_name;

    };

    /**
     * \brief A struct for containing information about a RAPID task program
     * and motion pointer position.
     */
    struct RAPIDTaskPcpState
    {
        /**
           * \brief A constructor.
           *
           * \param program_pointer position where the program pointer is.
           * \param motion_pointer position where the motion pointer is.
         */
        RAPIDTaskPcpState(RAPIDPcpInfo program_pointer,
                          RAPIDPcpInfo motion_pointer)
        :
        program_pointer(std::move(program_pointer)),
        motion_pointer(std::move(motion_pointer))
        {}

        /**
           * \brief Where the pointer position starts.
         */
        RAPIDPcpInfo program_pointer;
        /**
           * \brief Where the pointer position ends.
         */
        RAPIDPcpInfo motion_pointer;

    };


}