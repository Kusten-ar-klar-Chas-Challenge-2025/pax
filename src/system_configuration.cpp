#include "system_configuration.h"
#include <EEPROM.h>  // Uno R4 WiFi EEPROM
#include "system_states.h"

// Array of reporting interval values
uint32_t SystemConfiguration::m_reporting_intervals[STATE_COUNT] = {
    5000,       // ACTIVE: 5 seconds
    10000,      // TRANSITION: 10 seconds
    60000,      // IDLE: 1 minute
    300000,     // AFTER_HOURS: 5 minutes
    1800000     // WEEKEND: 30 minutes
};

// Array of network interval values
uint32_t SystemConfiguration::m_network_intervals[STATE_COUNT] = {
    5000,       // ACTIVE: 5 seconds
    10000,      // TRANSITION: 10 seconds
    30000,      // IDLE: 30 seconds
    300000,     // AFTER_HOURS: 5 minutes
    1800000     // WEEKEND: 30 minutes
};

// Array of presence check interval values
uint32_t SystemConfiguration::m_presence_check_intervals[STATE_COUNT] = {
    100,        // ACTIVE: 100 ms
    100,        // TRANSITION: 100 ms
    500,        // IDLE: 500 ms
    5000,       // AFTER_HOURS: 5 seconds
    10000       // WEEKEND: 10 seconds
};

SystemConfiguration::SystemConfiguration() :
    // Default office hours (8:30 AM to 5:30 PM)
    m_office_hours{8, 30, 17, 30},
    
    // Default transition duration (30 seconds)
    m_transition_hold_ms(30000),
    
    // Default PIR hold duration (5 seconds)
    m_pir_hold_duration_ms(5000),
    
    // Default NTP sync interval (24 hours)
    m_ntp_sync_interval_ms(86400000),
    
    // Default configuration URL
    m_configuration_url("https://example.com/api/config"),
    
    // Last config update time
    m_last_config_update_ms(0),
    
    // Default config update interval (1 hour)
    m_config_update_interval_ms(3600000)
{
    // Constructor body intentionally empty - initialization done in the initializer list
}

uint32_t SystemConfiguration::get_reporting_interval(const uint8_t state_index) const
{
    if (state_index < STATE_COUNT)
    {
        return m_reporting_intervals[state_index];
    }
    return m_reporting_intervals[static_cast<size_t>(system_state_t::IDLE)]; // Default to idle interval
}

void SystemConfiguration::set_reporting_interval(uint8_t state_index, uint32_t interval_ms) const {
    if (state_index < STATE_COUNT)
    {
        m_reporting_intervals[state_index] = interval_ms;
    }
}

uint32_t SystemConfiguration::get_network_interval(uint8_t state_index) const
{
    if (state_index < STATE_COUNT)
    {
        return m_network_intervals[state_index];
    }
    return m_network_intervals[static_cast<size_t>(system_state_t::IDLE)]; // Default to idle interval
}

void SystemConfiguration::set_network_interval(uint8_t state_index, uint32_t interval_ms)
{
    if (state_index < STATE_COUNT)
    {
        m_network_intervals[state_index] = interval_ms;
    }
}

uint32_t SystemConfiguration::get_presence_check_interval(uint8_t state_index) const
{
    if (state_index < STATE_COUNT)
    {
        return m_presence_check_intervals[state_index];
    }
    return m_presence_check_intervals[static_cast<size_t>(system_state_t::IDLE)]; // Default to idle interval
}

void SystemConfiguration::set_presence_check_interval(const uint8_t state_index, const uint32_t interval_ms)
{
    if (state_index < STATE_COUNT)
    {
        m_presence_check_intervals[state_index] = interval_ms;
    }
}

bool SystemConfiguration::parse_configuration(const String& json_string)
{
    // Create a JSON document
    // Adjust capacity based on the expected size of your JSON configuration
    JsonDocument doc;
    
    // Deserialize the JSON document
    DeserializationError const error = deserializeJson(doc, json_string);
    
    // Check for parsing errors
    if (error)
    {
        Serial.print(F("JSON parsing failed: "));
        Serial.println(error.c_str());
        return false;
    }
    
    // Parse office hours
    if (doc["office_hours"].is<JsonObject>())
    {
        JsonObject const office_hours = doc["office_hours"];
        m_office_hours.start_hour = office_hours["start_hour"] | m_office_hours.start_hour;
        m_office_hours.start_minute = office_hours["start_minute"] | m_office_hours.start_minute;
        m_office_hours.end_hour = office_hours["end_hour"] | m_office_hours.end_hour;
        m_office_hours.end_minute = office_hours["end_minute"] | m_office_hours.end_minute;
    }
    
    // Parse transition hold duration
    m_transition_hold_ms = doc["transition_hold_ms"] | m_transition_hold_ms;
    
    // Parse PIR hold duration
    m_pir_hold_duration_ms = doc["pir_hold_duration_ms"] | m_pir_hold_duration_ms;
    
    // Parse intervals using the array of state names
    if (doc["reporting_intervals"].is<JsonObject>())
    {
        JsonObject intervals = doc["reporting_intervals"];
        for (uint8_t i = 0; i < STATE_COUNT; i++)
        {
            if (intervals[STATE_NAMES[i]].is<uint32_t>())
            {
                m_reporting_intervals[i] = intervals[STATE_NAMES[i]] | m_reporting_intervals[i];
            }
        }
    }
    
    if (doc["network_intervals"].is<JsonObject>())
    {
        JsonObject const intervals = doc["network_intervals"];
        for (uint8_t i = 0; i < STATE_COUNT; i++)
        {
            if (intervals[STATE_NAMES[i]].is<uint32_t>())
            {
                m_network_intervals[i] = intervals[STATE_NAMES[i]] | m_network_intervals[i];
            }
        }
    }
    
    if (doc["presence_check_intervals"].is<JsonObject>())
    {
        JsonObject const intervals = doc["presence_check_intervals"];
        for (uint8_t i = 0; i < STATE_COUNT; i++)
        {
            if (intervals[STATE_NAMES[i]].is<uint32_t>())
            {
                m_presence_check_intervals[i] = intervals[STATE_NAMES[i]] | m_presence_check_intervals[i];
            }
        }
    }
    
    // Parse NTP sync interval
    m_ntp_sync_interval_ms = doc["ntp_sync_interval_ms"] | m_ntp_sync_interval_ms;
    
    // Parse configuration URL
    if (doc["config_url"].is<String>() && doc["config_url"].as<String>().length() > 0)
    {
        m_configuration_url = doc["config_url"].as<String>();
    }
    
    // Parse configuration update interval
    m_config_update_interval_ms = doc["config_update_interval_ms"] | m_config_update_interval_ms;
    
    // Mark as updated
    mark_updated();
    
    return true;
}

bool SystemConfiguration::fetch_remote_configuration(Client* client)
{
    // HTTP client for fetching configuration
    // Note: We're using a simple implementation for clarity
    // In a production environment, consider using a more robust HTTP client library
    
    // Parse URL to extract host and path
    String host;
    String path;
    
    // Very simple URL parsing (assumes an http or https URL format)
    if (const int protocol_end = m_configuration_url.indexOf("://");
        protocol_end > 0)
    {
        int const host_start = protocol_end + 3;

        if (int const path_start = m_configuration_url.indexOf('/', host_start);
            path_start > 0)
        {
            host = m_configuration_url.substring(host_start, path_start);
            path = m_configuration_url.substring(path_start);
        }
        else
        {
            host = m_configuration_url.substring(host_start);
            path = "/";
        }
    }
    else
    {
        // Invalid URL format
        Serial.println(F("Invalid URL format"));
        return false;
    }
    
    // Connect to server
    if (!client->connect(host.c_str(), 80))
    {
        Serial.println(F("Connection failed"));
        return false;
    }
    
    // Send HTTP request
    client->print(F("GET "));
    client->print(path);
    client->println(F(" HTTP/1.1"));
    client->print(F("Host: "));
    client->println(host);
    client->println(F("Connection: close"));
    client->println();
    
    // Wait for server response
    const uint32_t timeout = millis();
    while (client->available() == 0)
    {
        if (millis() - timeout > 5000)
        {
            Serial.println(F("Client timeout"));
            client->stop();
            return false;
        }
        delay(10);
    }
    
    // Skip HTTP headers
    bool is_body = false;
    String response;
    
    while (client->available())
    {
        String line = client->readStringUntil('\r');
        client->read(); // Consume the '\n'
        
        if (!is_body)
        {
            // An empty line indicates the end of headers
            if (line.length() == 0)
            {
                is_body = true;
            }
        }
        else
        {
            // Append to response body
            response += line;
        }
    }
    
    // Close connection
    client->stop();
    
    // Parse configuration
    if (response.length() > 0)
    {
        return parse_configuration(response);
    }
    
    return false;
}

bool SystemConfiguration::save_configuration() const
{
    // Create a JSON document to serialize the configuration
    JsonDocument doc;
    
    // Add office hours
    JsonObject const office_hours = doc["office_hours"].to<JsonObject>();
    office_hours["start_hour"] = m_office_hours.start_hour;
    office_hours["start_minute"] = m_office_hours.start_minute;
    office_hours["end_hour"] = m_office_hours.end_hour;
    office_hours["end_minute"] = m_office_hours.end_minute;
    
    // Add transition hold duration
    doc["transition_hold_ms"] = m_transition_hold_ms;
    
    // Add PIR hold duration
    doc["pir_hold_duration_ms"] = m_pir_hold_duration_ms;
    
    // Add intervals using the array of state names
    const JsonObject reporting_intervals = doc["reporting_intervals"].to<JsonObject>();
    const JsonObject network_intervals = doc["network_intervals"].to<JsonObject>();
    const JsonObject presence_intervals = doc["presence_check_intervals"].to<JsonObject>();
    
    for (uint8_t i = 0; i < STATE_COUNT; i++)
    {
        reporting_intervals[STATE_NAMES[i]] = m_reporting_intervals[i];
        network_intervals[STATE_NAMES[i]] = m_network_intervals[i];
        presence_intervals[STATE_NAMES[i]] = m_presence_check_intervals[i];
    }
    
    // Add NTP sync interval
    doc["ntp_sync_interval_ms"] = m_ntp_sync_interval_ms;
    
    // Add configuration URL
    doc["config_url"] = m_configuration_url;
    
    // Add configuration update interval
    doc["config_update_interval_ms"] = m_config_update_interval_ms;
    
    // Serialize to a string
    String config_json;
    serializeJson(doc, config_json);
    

    //! TODO: Actually storing the object to flash memory

    
    // Write configuration to serial for now (as a fallback)
    Serial.println(F("Configuration would be saved to flash storage:"));
    Serial.println(config_json);
    

    
    // Return true to indicate success (for compatibility)
    return true;
}

bool SystemConfiguration::load_configuration()
{

    //! TODO: Flash storage implementation
    
    Serial.println(F("Loading default configuration (flash storage not implemented)"));
    
    // Return false to indicate that we're using defaults
    return false;
}