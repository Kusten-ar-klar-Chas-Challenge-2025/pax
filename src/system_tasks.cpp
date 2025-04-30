#include "system_tasks.h"
#include <RTC.h>


// External global objects (declared in main.cpp)
extern NetworkingBase network;
extern MeasurementState measurement_state;
extern SystemStateManager state_manager;
extern SystemConfiguration g_config;

// Time tracking for tasks
static uint32_t last_time_check_ms = 0;
static uint32_t last_network_check_ms = 0;
static uint32_t last_config_check_ms = 0;

namespace Tasks
{
    void init()
    {
        // Initialize timestamps with the current time
        const uint32_t current_time = millis();
        last_time_check_ms = current_time;
        last_network_check_ms = current_time;
        last_config_check_ms = current_time;
        
        // Initialize RTC with network time
        Time::init();
    }
    
    namespace Time
    {
        void print_current()
        {
            // Getting a time object from R4 clock
            RTCTime current_time;
            RTC.getTime(current_time);

            Serial.print(F("Current time: "));
            Serial.print(current_time);

        }
        
        void init()
        {
            // Start the RTC
            RTC.begin();
            
            // Set initial time from NTP
            sync_from_network(network);
            
            // Display current time
            print_current();
        }
        
        // ReSharper disable once CppDFAConstantFunctionResult
        bool sync_from_network(const NetworkingBase& network)
        {
            Serial.println(F("Attempting to synchronize time..."));
            
            if (!network.ready_for_traffic())
            {
                Serial.println(F("Network not ready, using existing time"));
                return false;
            }
            
            //! TODO: First, get time from backend using some /time endpoint, using existing network
            //! TODO: If that succeeds, great!
            //! TODO: As fallback, could use ntp? Need to set up UDP somehow, might need to extend network.
            if constexpr ( false ) {
                return true;
            }
            
            Serial.println(F("Time synchronization failed"));
            return false;
        }
        
        void update(SystemStateManager& state_manager, NetworkingBase& network)
        {
            if (const uint32_t current_ms = millis(); current_ms - last_time_check_ms >= 60000)  // Check every minute
            {
                last_time_check_ms = current_ms;
                
                if (state_manager.update_time_based_state())
                {
                    Serial.print(F("Time state changed to: "));
                    Serial.println(state_manager.get_state_name());
                    state_manager.force_status_report();
                }
                
                // Periodically print current time
                print_current();
                
                // Periodic NTP time synchronization to prevent drift
                if (state_manager.get_time_since_ntp_sync() >= g_config.get_ntp_sync_interval_ms())
                {
                    // Only sync during idle periods to avoid disrupting active operations
                    if (const system_state_t current_state = state_manager.get_state();
                        current_state == system_state_t::IDLE ||
                        current_state == system_state_t::AFTER_HOURS ||
                        current_state == system_state_t::WEEKEND)
                    {
                        sync_from_network(network);
                    }
                }
            }
        }
    }
    
    namespace Config
    {
        void update(SystemConfiguration& config, SystemStateManager& state_manager, const NetworkingBase& network)
        {
            // Only check for updates at the configured interval
            if (const uint32_t current_ms = millis(); current_ms - last_config_check_ms >= config.get_config_update_interval_ms())
            {
                last_config_check_ms = current_ms;
                
                // Only update if the network is available and the system is in a less active state
                if (network.ready_for_traffic() && 
                    (state_manager.get_state() == system_state_t::IDLE || 
                     state_manager.get_state() == system_state_t::AFTER_HOURS ||
                     state_manager.get_state() == system_state_t::WEEKEND))
                {
                    Serial.println(F("Checking for remote configuration updates..."));
                    
                    // Get client from network manager
                    if ( Client* client_ptr = network.current_client() )
                    {
                        if (config.fetch_remote_configuration(client_ptr))
                        {
                            Serial.println(F("Remote configuration retrieved successfully"));
                            
                            // Save the updated configuration to storage
                            if ( config.save_configuration() ) {
                                Serial.println(" New configuration saved successfully");
                            }
                            else
                            {
                                Serial.println(" Could not save new configuration");
                            }
                            
                            // Force state manager to re-evaluate with the new configuration
                            state_manager.force_status_report();
                        }
                        else
                        {
                            Serial.println(F("No new configuration available or update failed"));
                        }
                    }
                    else
                    {
                        Serial.println(F("Network not available for configuration update"));
                    }
                }
            }
        }
    }
    
    namespace Presence
    {
        void update(SystemStateManager& state_manager, MeasurementState& measurement, NetworkingBase& network)
        {
            // Update occupancy-based state
            if (state_manager.update_occupancy_based_state())
            {
                Serial.print(F("Occupancy state changed to: "));
                Serial.println(state_manager.get_state_name());
            }
            
            // Check if it's time to report status
            if (state_manager.should_report_status())
            {
                if (network.ready_for_traffic())
                {
                    bool room_occupied = measurement.roomHasActivity();
                    
                    Serial.print(F("Reporting: "));
                    Serial.print(room_occupied ? F("OCCUPIED") : F("VACANT"));
                    Serial.print(F(" in "));
                    Serial.print(state_manager.get_state_name());
                    Serial.println(F(" mode"));
                    
                    // This is where you would send the update to your backend
                    // Using the client from network manager:
                    // Client* client_ptr = network.current_client();
                    // if (client_ptr) {
                    //    send_occupancy_update(client_ptr, room_occupied, state_manager.get_state());
                    // }
                    
                    // Mark the status as reported
                    state_manager.status_reported(room_occupied);
                }
            }
        }
    }
    
    namespace Network
    {
        void update(const SystemStateManager& state_manager, NetworkingBase& network)
        {
            const uint32_t current_ms = millis();

            if (const uint32_t network_interval = state_manager.get_network_interval();
                current_ms - last_network_check_ms >= network_interval)
            {
                last_network_check_ms = current_ms;
                
                Serial.print(F("Network check ("));
                Serial.print(state_manager.get_state_name());
                Serial.println(F(" mode)"));
                
                network();  // Run network maintenance
            }
        }
    }
    
    void run_all()
    {
        // Execute all tasks in sequence
        Config::update(g_config, state_manager, network);
        Time::update(state_manager, network);
        Presence::update(state_manager, measurement_state, network);
        Network::update(state_manager, network);
    }
}