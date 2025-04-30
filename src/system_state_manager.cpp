#include "system_state_manager.h"
#include "system_configuration.h"

// External configuration object
extern SystemConfiguration g_config;


SystemStateManager::SystemStateManager(
    MeasurementState* measurement_state_ptr,
    const time_window_t& office_hours,
    const uint8_t presence_led_pin
) :
    m_current_state(system_state_t::IDLE),
    m_office_hours(office_hours),
    m_last_state_change_ms(0),
    m_last_presence_check_ms(0),
    m_last_presence_report_ms(0),
    m_last_time_check_ms(0),
    m_last_ntp_sync_ms(0),
    m_last_reported_presence(false),
    m_measurement_state_ptr(measurement_state_ptr),
    m_presence_led_enabled(presence_led_pin > 0),
    m_presence_led_pin(presence_led_pin)
{
    // Constructor body intentionally empty - initialization done in initializer list
}

void SystemStateManager::begin()
{
    // Configure LED pin if enabled
    if (m_presence_led_enabled)
    {
        pinMode(m_presence_led_pin, OUTPUT);
        digitalWrite(m_presence_led_pin, LOW);
    }
    
    // Initialize timestamps with the current time
    uint32_t current_time = millis();
    m_last_state_change_ms = current_time;
    m_last_presence_check_ms = current_time;
    m_last_presence_report_ms = current_time;
    m_last_time_check_ms = current_time;
    
    // Use configuration if available
    m_office_hours = g_config.get_office_hours();
    
    // Initial state is determined by time
    update_time_based_state();
}


bool SystemStateManager::is_time_within_window(
    uint8_t hour, 
    uint8_t minute, 
    const time_window_t& window
) const
{
    uint16_t time_minutes = hour * 60 + minute;
    uint16_t start_minutes = window.start_hour * 60 + window.start_minute;
    uint16_t end_minutes = window.end_hour * 60 + window.end_minute;
    
    return (time_minutes >= start_minutes && time_minutes <= end_minutes);
}

bool SystemStateManager::is_weekend()
{
    // Get current day of week from RTC
    RTCTime current_time;
    RTC.getTime(current_time);
    DayOfWeek current_day = current_time.getDayOfWeek();
    
    return (current_day == DayOfWeek::SATURDAY || current_day == DayOfWeek::SUNDAY);
}

bool SystemStateManager::is_within_office_hours()
{
    // First check if it's a weekend - if so, we're never in office hours
    if (is_weekend())
    {
        return false;
    }
    
    // Get current time components from the RTC
    RTCTime current_time;
    RTC.getTime(current_time);

    return is_time_within_window(current_time.getHour(), current_time.getMinutes(), m_office_hours);
}


bool SystemStateManager::is_interval_elapsed(uint32_t last_time, uint32_t interval) {
    return (millis() - last_time >= interval);
}

bool SystemStateManager::change_state(system_state_t new_state)
{
    if (m_current_state != new_state)
    {
        m_current_state = new_state;
        m_last_state_change_ms = millis();
        return true;  // State was changed
    }
    return false;  // No change occurred
}

void SystemStateManager::update_presence_led(const bool room_occupied) const {
    if (m_presence_led_enabled)
    {
        digitalWrite(m_presence_led_pin, room_occupied ? HIGH : LOW);
    }
}

uint32_t SystemStateManager::get_presence_check_interval() const
{
    return g_config.get_presence_check_interval(static_cast<uint8_t>(m_current_state));
}


uint32_t SystemStateManager::get_reporting_interval() const
{
    return g_config.get_reporting_interval(static_cast<uint8_t>(m_current_state));
}


uint32_t SystemStateManager::get_network_interval() const
{
    return g_config.get_network_interval(static_cast<uint8_t>(m_current_state));
}

const char* SystemStateManager::get_state_name() const
{
    return STATE_NAMES[static_cast<size_t>(m_current_state)];
}

bool SystemStateManager::update_time_based_state()
{
    bool state_changed = false;
    
    // Update time check timestamp
    m_last_time_check_ms = millis();
    
    // Determine the appropriate state based on time
    if (is_weekend())
    {
        state_changed = change_state(system_state_t::WEEKEND);
    }
    else if (!is_within_office_hours())
    {
        if (m_current_state != system_state_t::AFTER_HOURS && 
            m_current_state != system_state_t::WEEKEND)
        {
            state_changed = change_state(system_state_t::AFTER_HOURS);
        }
    }
    else
    {
        // During office hours, reset to IDLE if we were in a time-based state
        if (m_current_state == system_state_t::AFTER_HOURS || 
            m_current_state == system_state_t::WEEKEND)
        {
            state_changed = change_state(system_state_t::IDLE);
        }
    }
    
    return state_changed;
}

bool SystemStateManager::update_occupancy_based_state()
{
    if (!m_measurement_state_ptr)
    {
        return false;  // Can't update without a valid measurement state object
    }
    
    bool state_changed = false;
    
    // Check if it's time to scan for presence
    if (is_interval_elapsed(m_last_presence_check_ms, get_presence_check_interval()))
    {
        m_last_presence_check_ms = millis();
        
        // Update measurement state
        m_measurement_state_ptr->update();
        bool room_occupied = m_measurement_state_ptr->roomHasActivity();
        
        // Update LED indicator
        update_presence_led(room_occupied);
        
        // Only update occupancy-based state during office hours
        if (m_current_state != system_state_t::AFTER_HOURS && 
            m_current_state != system_state_t::WEEKEND)
        {
            if (room_occupied && m_current_state != system_state_t::ACTIVE)
            {
                state_changed = change_state(system_state_t::ACTIVE);
            }
            else if (!room_occupied && m_current_state == system_state_t::ACTIVE)
            {
                state_changed = change_state(system_state_t::TRANSITION);
            }
            
            // Check transition timeout
            if (m_current_state == system_state_t::TRANSITION && 
                is_interval_elapsed(m_last_state_change_ms, g_config.get_transition_hold_ms()))
            {
                state_changed = change_state(system_state_t::IDLE);
            }
        }
    }
    
    return state_changed;
}

bool SystemStateManager::should_report_status(const bool force_immediate)
{
    if (!m_measurement_state_ptr)
    {
        return false;  // Can't report without a valid measurement state object
    }
    
    // Force immediate report if requested
    if (force_immediate)
    {
        m_last_presence_report_ms = 0;
    }
    
    // Check if it's time to report
    if (is_interval_elapsed(m_last_presence_report_ms, get_reporting_interval()))
    {
        // Determine if we should report based on state
        if (const bool room_occupied = m_measurement_state_ptr->roomHasActivity();
            room_occupied != m_last_reported_presence)
        {
            return true;  // Always report state changes
        }
        else if (m_current_state == system_state_t::AFTER_HOURS || 
                m_current_state == system_state_t::WEEKEND)
        {
            // For off-hours, report on schedule even without changes
            return true;
        }
        else if (force_immediate)
        {
            return true;
        }
    }
    
    return false;
}

void SystemStateManager::status_reported(const bool presence_state)
{
    m_last_presence_report_ms = millis();
    m_last_reported_presence = presence_state;
}