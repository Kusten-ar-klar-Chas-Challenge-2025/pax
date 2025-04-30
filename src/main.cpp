#include <Arduino.h>
#include "networking_base.h"
#include "measurement_state.h"
#include "system_state_manager.h"
#include "system_configuration.h"
#include "system_tasks.h"

// Pin definitions - avoiding SPI pins (10-13)
constexpr uint8_t STATUS_LED_PIN = 9;
constexpr uint8_t PIR_PIN = 2;

// Global configuration object
SystemConfiguration g_config;

// Global objects - accessed by tasks.cpp through extern
WiFiClient wifi_client;
EthernetClient ethernet_client;
NetworkingBase network(&wifi_client, &ethernet_client);  // Will be initialized in setup
MeasurementState measurement_state(PIR_PIN);  // PIR hold duration from config
SystemStateManager state_manager(&measurement_state, g_config.get_office_hours(), STATUS_LED_PIN);

void setup()
{
    // Initialize serial
    Serial.begin(115200);
    delay(100);
    
    Serial.println(F("Office Hours Room Monitor: Initializing..."));
    
    // Initialize network clients
    network = NetworkingBase(&wifi_client, &ethernet_client);
    
    // Try to load configuration from storage
    if (!g_config.load_configuration())
    {
        Serial.println(F("Failed to load configuration, using defaults"));
        // Save default configuration
        g_config.save_configuration();
    }
    
    // Set PIR hold duration from config
    measurement_state = MeasurementState(PIR_PIN, g_config.get_pir_hold_duration_ms());
    
    // Initialize components
    measurement_state.init();
    network.begin();
    state_manager.begin();
    
    // Initialize tasks system
    Tasks::init();
    
    Serial.println(F("Office Hours Room Monitor: Initialization complete"));
}

void loop()
{
    
    Tasks::run_all();
    
    // Small delay to prevent CPU overload
    delay(10);
}