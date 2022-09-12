#pragma once

#include <abb_librws/v1_0/rws_client.h>

#include <abb_librws/rws.h>
#include <abb_librws/common/rw/rapid.h>

#include <string>


namespace abb :: rws :: v1_0 :: rw
{
    using namespace rws::rw;
}


namespace abb :: rws :: v1_0 :: rw :: rapid
{
    /**
     * \brief A class for representing a RAPID symbol resource.
     */
    struct RAPIDSymbolResource
    {
        /**
         * \brief A constructor.
         *
         * \param module specifying the name of the RAPID module containing the symbol.
         * \param name specifying the name of the RAPID symbol.
         */
        RAPIDSymbolResource(const std::string& module, const std::string& name)
        :   module(module)
        ,   name(name)
        {}

        /**
         * \brief The RAPID module name.
         */
        std::string module;

        /**
         * \brief The RAPID symbol name.
         */
        std::string name;
    };


    /**
     * \brief A class for representing a RAPID resource.
     */
    struct RAPIDSubscribableResource: SubscribableResource
    {
        /**
         * \brief A constructor.
         *
         * \param task specifying the name of the RAPID task containing the symbol.
         * \param module specifying the name of the RAPID module containing the symbol.
         * \param name specifying the name of the RAPID symbol.
         */
        RAPIDSubscribableResource(const std::string& task, const std::string& module, const std::string& name)
        :   task(task)
        ,   module(module)
        ,   name(name)
        {}

        /**
         * \brief A constructor.
         *
         * \param task specifying the name of the RAPID task containing the symbol.
         * \param symbol specifying the names of the RAPID module and the the symbol.
         */
        RAPIDSubscribableResource(const std::string& task, const RAPIDSymbolResource& symbol)
        :   task(task)
        ,   module(symbol.module)
        ,   name(symbol.name)
        {}

        bool equals(const SubscribableResource& rhs) const override
        {
          return getURI() == rhs.getURI();
        }

        std::size_t getHash() const override
        { return std::hash<std::string>()(getURI()); }


        /**
         * \brief The RAPID task name.
         */
        std::string task;

        /**
         * \brief The RAPID module name.
         */
        std::string module;

        /**
         * \brief The RAPID symbol name.
         */
        std::string name;


        std::string getURI() const override;
    };


    /**
     * \brief RAPID execution state subscription resource
     */
    struct RAPIDExecutionStateSubscribableResource: SubscribableResource
    {
        explicit RAPIDExecutionStateSubscribableResource()
        {
        }

        bool equals(const SubscribableResource& rhs) const override
        {
          return getURI() == rhs.getURI();
        }

        std::size_t getHash() const override
        { return std::hash<std::string>()(getURI()); }

        std::string getURI() const override;
    };

    /**
     * \brief A function for retrieving the execution state of RAPID.
     *
     * https://developercenter.robotstudio.com/api/rwsApi/rapid_execution_get_page.html
     *
     * \param client RWS client
     *
     * \return Current RAPID execution state.
     *
     * \throw \a RWSError if something goes wrong.
     */
    RAPIDExecutionInfo getRAPIDExecution(RWSClient& client);

    /**
     * \brief A function for starting RAPID execution in the robot controller.
     *
     * \param client RWS client
     *
     * There can be a delay between the function returns and when the RAPID program enters the "running" state.
     *
     * \throw \a std::runtime_error if something goes wrong.
     */
    void startRAPIDExecution(RWSClient& client);

    /**
     * \brief A function for stopping RAPID execution in the robot controller.
     *
     * https://developercenter.robotstudio.com/api/rwsApi/rapid_execution_stop_page.html
     *
     * There can be a delay between the function returns and when the RAPID program enters the "stopped" state.
     *
     * \param client RWS client
     * \param stopmode stop mode
     * \param usetsp which tasks to stop (?)
     *
     * \throw \a std::runtime_error if something goes wrong.
     */
    void stopRAPIDExecution(RWSClient& client, StopMode stopmode = StopMode::stop, UseTsp usetsp = UseTsp::normal);

    /**
     * \brief A function for reseting the RAPID program pointer in the robot controller.
     *
     * \param client RWS client
     *
     * \throw \a std::runtime_error if something goes wrong.
     */
    void resetRAPIDProgramPointer(RWSClient& client);

    /**
     * \brief A function for retrieving information about the RAPID modules of a RAPID task defined in the robot controller.
     *
     * \param client RWS client
     *
     * \return \a std::vector<RAPIDModuleInfo> containing the RAPID modules information.
     *
     * \throw \a std::runtime_error if something goes wrong.
     */
    std::vector<RAPIDModuleInfo> getRAPIDModulesInfo(RWSClient& client, const std::string& task);

    /**
     * \brief A function for retrieving information about the RAPID tasks defined in the robot controller.
     *
     * \param client RWS client
     *
     * \return \a std::vector<RAPIDTaskInfo> containing the RAPID tasks information.
     *
     * \throw \a std::runtime_error if something goes wrong.
     */
    std::vector<RAPIDTaskInfo> getRAPIDTasks(RWSClient& client);

    /**
     * \brief A function for retrieving the data of a RAPID symbol.
     *
     * \param client RWS client
     * \param resource specifies the RAPID task, module, and symbol name for the RAPID resource.
     * \param data variable for receiving the symbol value.
     *
     * \throw \a RWSError if something goes wrong.
     */
    void getRAPIDSymbolData(RWSClient& client, RAPIDResource const& resource, RAPIDSymbolDataAbstract& data);

    /**
     * \brief A function for retrieving the data of a RAPID symbol in raw text format.
     *
     * See the corresponding "setRAPIDSymbolData(...)" function for examples of RAPID symbols in raw text format.
     *
     * \param client RWS client
     * \param resource specifies the RAPID task, module, and symbol name for the RAPID resource.
     *
     * \return String containing the data.
     *
     * \throw \a RWSError if something goes wrong.
     */
    std::string getRAPIDSymbolData(RWSClient& client, RAPIDResource const& resource);

    /**
     * \brief A function for setting the data of a RAPID symbol (based on the provided struct representing the RAPID data).
     *
     * \param client RWS client
     * \param resource specifying the RAPID task, module and symbol names for the RAPID resource.
     * \param data for the RAPID symbol's new data.
     *
     * \throw \a RWSError if something goes wrong.
     */
    void setRAPIDSymbolData(RWSClient& client, const RAPIDResource& resource, const RAPIDSymbolDataAbstract& data);

    /**
     * \brief A function for setting the data of a RAPID symbol.
     *
     * \param client RWS client
     * \param resource specifying the RAPID task, module and symbol names for the RAPID resource.
     * \param data the RAPID symbol's new data.
     *
     * \throw \a RWSError if something goes wrong.
     */
    void setRAPIDSymbolData(RWSClient& client, const RAPIDResource& resource, const std::string& data);

    /**
     * \brief A function for loading a module to the robot controller.
     *
     * \param client RWS client
     * \param task specifying the RAPID task.
     * \param resource specifying the file's directory and name.
     * \param replace indicating if the actual module into the controller must be replaced by the new one or not.
     *
     * \throw \a std::exception if something goes wrong.
     */
    void loadModuleIntoTask(RWSClient& client, const std::string& task, const FileResource& resource, const bool replace = false);

    /**
     * \brief A function for unloading a module to the robot controller.
     *
     * \param client RWS client
     * \param task specifying the RAPID task.
     * \param resource specifying the file's directory and name.
     *
     * \throw \a std::exception if something goes wrong.
     */
    void unloadModuleFromTask(RWSClient& client, const std::string& task, const FileResource& resource);
}