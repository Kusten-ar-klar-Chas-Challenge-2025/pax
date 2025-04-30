#ifndef SYSTEM_TASKS_H
#define SYSTEM_TASKS_H

#include "measurement_state.h"
#include "system_state_manager.h"
#include "system_configuration.h"
#include "networking_base.h"

//! Tasks namespace to organize system functionality
namespace Tasks
{
    //! Initialize the task system
    void init();
    
    //! Time management namespace for RTC and time synchronization
    namespace Time
    {
        //! Initialize the RTC and set the initial time
        void init();
        
        //! Synchronize time from a NTP server
        //! @param network Reference to the network manager
        //! @return true if synchronization was successful
        bool sync_from_network(const NetworkingBase& network);
        
        //! Print the current time to the serial monitor
        void print_current();
        
        //! Update time-based state
        //! @param state_manager Reference to the state manager
        //! @param network Reference to the network manager
        void update(const SystemStateManager& state_manager, const NetworkingBase& network);
    }
    
    //! Configuration namespace for system settings management
    namespace Config
    {
        //! Update system configuration from a remote server
        //! @param config Reference to the configuration object
        //! @param state_manager Reference to the state manager
        //! @param network Reference to the network manager
        void update(SystemConfiguration& config, const SystemStateManager& state_manager, const NetworkingBase& network);
    }
    
    //! Presence namespace for occupancy detection and reporting
    namespace Presence
    {
        //! Update the presence state and report if needed
        //! @param state_manager Reference to the state manager
        //! @param measurement Reference to the measurement object
        //! @param network Reference to the network manager
        void update(const SystemStateManager& state_manager, const MeasurementState& measurement, const NetworkingBase& network);
    }
    
    //! Network namespace for connectivity management
    namespace Network
    {
        //! Perform network maintenance
        //! @param state_manager Reference to the state manager
        //! @param network Reference to the network manager
        void update(const SystemStateManager& state_manager, NetworkingBase& network);
    }
    
    //! Task manager operator to run all system tasks
    //! Call this from the main loop
    void run_all();
}

#endif // SYSTEM_TASKS_H