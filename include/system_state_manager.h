#ifndef SYSTEM_STATE_MANAGER_H
#define SYSTEM_STATE_MANAGER_H

#include <Arduino.h>
#include <RTC.h>
#include "measurement_state.h"
#include "system_states.h"
#include "time_types.h"

//! Class for managing system state based on time and occupancy
class SystemStateManager
{
private:
    system_state_t m_current_state;          //!< Current system state
    time_window_t m_office_hours;            //!< Configured office hours
    uint32_t m_last_state_change_ms;         //!< Time of last state change
    uint32_t m_last_presence_check_ms;       //!< Time of last presence check
    uint32_t m_last_presence_report_ms;      //!< Time of last presence report
    uint32_t m_last_time_check_ms;           //!< Time of last time-based check
    uint32_t m_last_ntp_sync_ms;             //!< Time of last NTP sync
    bool m_last_reported_presence;           //!< Last reported presence state
    MeasurementState* m_measurement_state_ptr; //!< Pointer to measurement state object
    bool m_presence_led_enabled;             //!< Whether to use presence LED indicator
    uint8_t m_presence_led_pin;              //!< Pin for presence LED (if enabled)
    
    //! Transition hold time in milliseconds (time to wait before moving from TRANSITION to IDLE)
    static constexpr uint32_t TRANSITION_HOLD_MS = 30000;
    
    //! Check if the current time is within defined office hours
    //! @return true if within office hours, false otherwise
    [[nodiscard]] bool is_within_office_hours() ;

    bool is_interval_elapsed(uint32_t last_time, uint32_t interval);

    //! Check if today is a weekend day
    //! @return true if weekend, false otherwise
    [[nodiscard]] bool is_weekend() ;

    bool change_state(system_state_t new_state);

    //! Update presence LED based on current occupancy
    //! @param room_occupied Whether the room is currently occupied
    void update_presence_led(bool room_occupied) const;
    
    //! Get the appropriate interval for presence checking based on the current state
    //! @return interval in milliseconds for presence checking
    [[nodiscard]] uint32_t get_presence_check_interval() const;
    
    //! Get the appropriate interval for status reporting based on the current state
    //! @return interval in milliseconds for status reporting
    [[nodiscard]] uint32_t get_reporting_interval() const;
    
public:
    //! Constructor
    //! @param measurement_state_ptr Pointer to the measurement state object
    //! @param office_hours Configured office hours
    //! @param presence_led_pin Pin for LED indicating presence status (or 0 to disable)
    SystemStateManager(
        MeasurementState* measurement_state_ptr,
        const time_window_t& office_hours,
        uint8_t presence_led_pin
    );
    
    //! Initialize the state manager
    void begin();

    [[nodiscard]] bool is_time_within_window(uint8_t hour, uint8_t minute, const time_window_t &window) const;

    //! Get the current system state
    //! @return Current system state
    [[nodiscard]] system_state_t get_state() const
    {
        return m_current_state;
    }
    
    //! Get a string representation of the current state for logging
    //! @return String describing the current state
    [[nodiscard]] const char* get_state_name() const;
    
    //! Update state based on time of day
    //! @return true if the state was changed, false otherwise
    bool update_time_based_state();
    
    //! Update state based on room occupancy
    //! @return true if the state was changed, false otherwise
    bool update_occupancy_based_state();
    
    //! Check if it's time to report status
    //! @param force_immediate Force an immediate report
    //! @return true if status should be reported, false otherwise
    bool should_report_status(bool force_immediate = false);
    
    //! Mark a status report as completed
    //! @param presence_state Current presence state being reported
    void status_reported(bool presence_state);
    
    //! Get the appropriate network maintenance interval for the current state
    //! @return Network maintenance interval in milliseconds
    [[nodiscard]] uint32_t get_network_interval() const;
    
    //! Get time since last NTP sync
    //! @return Time in milliseconds since the last NTP sync
    [[nodiscard]] uint32_t get_time_since_ntp_sync() const
    {
        return millis() - m_last_ntp_sync_ms;
    }
    
    //! Mark a successful NTP sync
    void mark_ntp_sync()
    {
        m_last_ntp_sync_ms = millis();
    }
    
    //! Force an immediate status report
    void force_status_report()
    {
        m_last_presence_report_ms = 0;
    }
};

#endif // SYSTEM_STATE_MANAGER_H