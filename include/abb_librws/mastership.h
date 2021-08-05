#pragma once


namespace abb :: rws
{
    /// @brief Provides functions to request an release RWS mastership.
    ///
    class MastershipManager
    {
    public:
        /**
         * TODO(mwojcik): documentation
         */
        virtual void requestMastership() = 0;

        /**
         * TODO(mwojcik): documentation
         */
        virtual void releaseMastership() = 0;
    };


    /// @brief Holds RWS mastership permissions as long as the object exists.
    ///
    class Mastership
    {
    public:
        /// @brief Requests mastership permissions.
        ///
        /// @param comm communication channel with the robot.
        ///
        explicit Mastership(MastershipManager& mastership_manager);

        /// @brief Releases mastership permissions.
        ///
        ~Mastership();

    private:
        MastershipManager& mastership_manager_;
    };
}
