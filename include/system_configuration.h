#ifndef SYSTEM_CONFIGURATION_H
#define SYSTEM_CONFIGURATION_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "networking_base.h"
#include "system_states.h"
#include "time_types.h"


//! System configuration class to centralize all configurable parameters
class SystemConfiguration
{
private:
    //! Office hours configuration
    time_window_t m_office_hours;
    
    //! Transition held duration in milliseconds
    uint32_t m_transition_hold_ms;
    
    //! PIR sensor hold duration in milliseconds
    uint32_t m_pir_hold_duration_ms;
    
    //! Reporting intervals by state in milliseconds
    static uint32_t m_reporting_intervals[STATE_COUNT];
    
    //! Network check intervals by state in milliseconds
    static uint32_t m_network_intervals[STATE_COUNT];
    
    //! Presence checking intervals by state in milliseconds
    static uint32_t m_presence_check_intervals[STATE_COUNT];
    
    //! NTP sync interval in milliseconds
    uint32_t m_ntp_sync_interval_ms;
    
    //! URL for remote configuration endpoint
    String m_configuration_url;
    
    //! Last configuration update timestamp
    uint32_t m_last_config_update_ms;
    
    //! Configuration update interval in milliseconds
    uint32_t m_config_update_interval_ms;
    
    //! Parse configuration from JSON string
    //! @param json_string JSON configuration string
    //! @return true if parsing was successful, false otherwise
    bool parse_configuration(const String& json_string);
    
public:
    //! Constructor with default values
    SystemConfiguration();
    
    //! Get office hours configuration
    [[nodiscard]] time_window_t get_office_hours() const
    {
        return m_office_hours;
    }
    
    //! Set office hours configuration
    void set_office_hours(const time_window_t& office_hours)
    {
        m_office_hours = office_hours;
    }
    
    //! Get transition hold duration in milliseconds
    [[nodiscard]] uint32_t get_transition_hold_ms() const
    {
        return m_transition_hold_ms;
    }
    
    //! Set transition hold duration in milliseconds
    void set_transition_hold_ms(uint32_t hold_ms)
    {
        m_transition_hold_ms = hold_ms;
    }
    
    //! Get PIR sensor hold duration in milliseconds
    [[nodiscard]] uint32_t get_pir_hold_duration_ms() const
    {
        return m_pir_hold_duration_ms;
    }
    
    //! Set PIR sensor hold duration in milliseconds
    void set_pir_hold_duration_ms(uint32_t hold_ms)
    {
        m_pir_hold_duration_ms = hold_ms;
    }
    
    //! Get the reporting interval for a specific state
    [[nodiscard]] uint32_t get_reporting_interval(uint8_t state_index) const;
    
    //! Set the reporting interval for a specific state
    void set_reporting_interval(uint8_t state_index, uint32_t interval_ms) const;
    
    //! Get the network check interval for a specific state
    [[nodiscard]] uint32_t get_network_interval(uint8_t state_index) const;
    
    //! Set network check interval for a specific state
    void set_network_interval(uint8_t state_index, uint32_t interval_ms);
    
    //! Get the presence check interval for a specific state
    [[nodiscard]] uint32_t get_presence_check_interval(uint8_t state_index) const;
    
    //! Set the presence check interval for a specific state
    static void set_presence_check_interval(uint8_t state_index, uint32_t interval_ms);
    
    //! Get NTP sync interval in milliseconds
    [[nodiscard]] uint32_t get_ntp_sync_interval_ms() const
    {
        return m_ntp_sync_interval_ms;
    }
    
    //! Set NTP sync interval in milliseconds
    void set_ntp_sync_interval_ms(uint32_t interval_ms)
    {
        m_ntp_sync_interval_ms = interval_ms;
    }
    
    //! Set configuration URL
    void set_configuration_url(const String& url)
    {
        m_configuration_url = url;
    }
    
    //! Get configuration URL
    [[nodiscard]] String get_configuration_url() const
    {
        return m_configuration_url;
    }
    
    //! Set configuration update interval
    void set_config_update_interval_ms(uint32_t interval_ms)
    {
        m_config_update_interval_ms = interval_ms;
    }
    
    //! Get configuration update interval
    [[nodiscard]] uint32_t get_config_update_interval_ms() const
    {
        return m_config_update_interval_ms;
    }
    
    //! Fetch configuration from remote endpoint
    //! @param client Reference to the network client for HTTP request
    //! @return true if fetch was successful, false otherwise
    bool fetch_remote_configuration(Client* client);
    
    //! Check if a configuration update is due
    //! @return true if the update interval has elapsed, false otherwise
    [[nodiscard]] bool is_update_due() const
    {
        return millis() - m_last_config_update_ms >= m_config_update_interval_ms;
    }
    
    //! Mark configuration as updated
    void mark_updated()
    {
        m_last_config_update_ms = millis();
    }
    
    //! Save configuration to flash storage
    //! @return true if saving was successful, false otherwise
    [[nodiscard]] bool save_configuration() const;
    
    //! Load configuration from flash storage
    //! @return true if loading was successful, false otherwise
    bool load_configuration();
};

#endif // SYSTEM_CONFIGURATION_H