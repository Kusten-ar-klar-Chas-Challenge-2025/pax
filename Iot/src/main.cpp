/**
 * @file main.cpp
 * @brief Global variables, setup() and main() for pax IoT
 * 
 */
#include "arduino_secrets.h"
#include "measurement_state.h"
#include "Backend.h"
#include <Arduino.h>
#include <WiFiS3.h>

// Comment this line out to disable verbose state debug logging - OR define via build flags
// #define MAIN_STATE_DEBUG

/**
 * @file arduino_secrets.h
 * @brief Configuration for WiFi and server connection
 * 
 * This file should contain the following defines:
 * @code
 * #define SECRET_SSID "your_ssid"
 * #define SECRET_PASS "your_password"
 * 
 * #define SERVER_URL "google.com"
 * #define SERVER_PORT 443  // Default HTTPS port
 * #define API_PATH "" 
 * 
 * #define JWT_USER "user"
 * #define JWT_PASS "password"
 * 
 * #define DEVICE_UUID "UUID2"  // UUID for initial registration
 * #define ROOM_OVERRIDE 37     // Optional room ID override for testing
 * @endcode
 */

int pir_pin = 2;  //!< PIR sensor pin
int led_pin = 3;  //!< LED indicator pin
int temp_pin = 6; //!< Temperature sensor pin
MeasurementState room_state(pir_pin, 60*1000, temp_pin); //!< Room state manager with PIR timeout and temperature sensor

// Server configuration
ServerInfo server_info(
    SERVER_URL,          //!< Server base URL
    SERVER_PORT,         //!< Server port
    API_PATH,            //!< API base path
    JWT_USER,            //!< JWT username
    JWT_PASS,            //!< JWT password
    DEVICE_UUID          //!< Device UUID
);

Backend backend(server_info);  //!< Backend communication manager

// Timing constants
constexpr time_t UPDATE_WAIT_TIME = 30 * 1000;  //!< Time between room state updates (30 seconds)
constexpr time_t ROOM_ID_RETRY_TIME = 10 * 60 * 1000;  //!< Time between room ID retry attempts (10 minutes)
constexpr time_t JWT_RETRY_TIME = 5 * 60 * 1000;  //!< Time between JWT token retry attempts (5 minutes)
time_t last_update = 0;  //!< Last successful room state update timestamp
time_t last_room_id_attempt = 0;  //!< Last room ID request attempt timestamp
time_t last_jwt_attempt = 0;  //!< Last JWT token request attempt timestamp
constexpr uint32_t serial_baud_rate = 115200;  //!< Serial communication baud rate

// System states
enum class SystemState {
    UNINITIALIZED,      //!< ServerInfo not fully constructed
    INITIALIZED,        //!< Basic initialization complete
    WIFI_CONNECT_ATTEMPTED, //!< WiFi connection attempt started
    WIFI_CONNECTED,     //!< WiFi connected and verified
    SERVER_CONNECTED,   //!< Connected to server (HEAD request succeeds)
    JWT_VALID,         //!< Has valid JWT token
    ROOM_CONFIGURED,    //!< Has valid room ID
    ERROR              //!< Error state
};

// Current system state
SystemState current_state = SystemState::UNINITIALIZED;

// Helper function to log state transitions
void log_state_transition(SystemState from, SystemState to, const char* reason = nullptr) {
#ifdef MAIN_STATE_DEBUG
    Serial.print("State transition: ");
    switch (from) {
        case SystemState::UNINITIALIZED: Serial.print("UNINITIALIZED"); break;
        case SystemState::INITIALIZED: Serial.print("INITIALIZED"); break;
        case SystemState::WIFI_CONNECT_ATTEMPTED: Serial.print("WIFI_CONNECT_ATTEMPTED"); break;
        case SystemState::WIFI_CONNECTED: Serial.print("WIFI_CONNECTED"); break;
        case SystemState::SERVER_CONNECTED: Serial.print("SERVER_CONNECTED"); break;
        case SystemState::JWT_VALID: Serial.print("JWT_VALID"); break;
        case SystemState::ROOM_CONFIGURED: Serial.print("ROOM_CONFIGURED"); break;
        case SystemState::ERROR: Serial.print("ERROR"); break;
    }
    Serial.print(" -> ");
    switch (to) {
        case SystemState::UNINITIALIZED: Serial.print("UNINITIALIZED"); break;
        case SystemState::INITIALIZED: Serial.print("INITIALIZED"); break;
        case SystemState::WIFI_CONNECT_ATTEMPTED: Serial.print("WIFI_CONNECT_ATTEMPTED"); break;
        case SystemState::WIFI_CONNECTED: Serial.print("WIFI_CONNECTED"); break;
        case SystemState::SERVER_CONNECTED: Serial.print("SERVER_CONNECTED"); break;
        case SystemState::JWT_VALID: Serial.print("JWT_VALID"); break;
        case SystemState::ROOM_CONFIGURED: Serial.print("ROOM_CONFIGURED"); break;
        case SystemState::ERROR: Serial.print("ERROR"); break;
    }
    if (reason) {
        Serial.print(" (");
        Serial.print(reason);
        Serial.print(")");
    }
    Serial.println();
#endif
}

// Helper function to update state
void update_state(SystemState new_state, const char* reason = nullptr) {
    if (current_state != new_state) {
        log_state_transition(current_state, new_state, reason);
        current_state = new_state;
    }
}

/**
 * @brief Attempt to get a room ID from the server
 * @return true if successful, false otherwise
 */
bool try_get_room_id() {
    if (backend.has_room_id()) {
        return true;
    }

    if (backend.get_room_config()) {
        if (server_info.room_id != 0) {
#ifdef MAIN_STATE_DEBUG
            Serial.print("Got room ID: ");
            Serial.println(server_info.room_id);
#endif
            return true;
        }
    }
#ifdef MAIN_STATE_DEBUG
    Serial.println("Failed to get room ID");
#endif
    return false;
}

void setup() {
    Serial.begin(serial_baud_rate);
    while (!Serial) {
        delay(50);
    }

    update_state(SystemState::INITIALIZED, "Serial initialized");

    //! Setting room ID to a specific number for testing
    #ifdef ROOM_OVERRIDE
    backend.update_room_id(ROOM_OVERRIDE);
    // update_state(SystemState::ROOM_CONFIGURED, "Room ID override set"); // This state update might be too early, let natural flow handle it or place after JWT_VALID
    #endif

    delay(500); // extra delay to give Serial connection time
    Wire.begin();
#ifdef MAIN_STATE_DEBUG
    Serial.println(F("System: Initializing room state"));
#endif
    room_state.begin();
#ifdef MAIN_STATE_DEBUG
    Serial.println(F("System: Initializing WiFi"));
#endif
    WiFi.begin(SECRET_SSID, SECRET_PASS);
    update_state(SystemState::WIFI_CONNECT_ATTEMPTED, "WiFi connection started");
    
    // Wait for WiFi connection with timeout
    const unsigned long wifi_timeout = 20000; // 20 second timeout
    const unsigned long start_time = millis();
    while (WiFi.status() != WL_CONNECTED) {
        if (millis() - start_time > wifi_timeout) {
#ifdef MAIN_STATE_DEBUG
            Serial.println("\nWiFi connection timeout");
#endif
            update_state(SystemState::ERROR, "WiFi connection timeout");
            return;
        }
        delay(500);
#ifdef MAIN_STATE_DEBUG
        Serial.print(".");
#endif
    }
    
    // Verify connection is stable
    delay(1000); // Give connection time to stabilize
    if (WiFi.status() == WL_CONNECTED) {
#ifdef MAIN_STATE_DEBUG
        Serial.println("\nConnected to WiFi");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
#endif
        update_state(SystemState::WIFI_CONNECTED, "WiFi connected and verified");
    } else {
#ifdef MAIN_STATE_DEBUG
        Serial.println("\nWiFi connection failed");
#endif
        update_state(SystemState::ERROR, "WiFi connection failed");
        return;
    }
#ifdef MAIN_STATE_DEBUG
    Serial.println(F("System: Configuring I/O pins"));
#endif
    pinMode(led_pin, OUTPUT);
    digitalWrite(led_pin, LOW);
    
    delay(100);
#ifdef MAIN_STATE_DEBUG
    Serial.println(F("System: Initialization complete"));
#endif
    
    // Initialize backend and get JWT token
#ifdef MAIN_STATE_DEBUG
    Serial.println(F("\nSystem: Initializing backend..."));
#endif
    if (!backend.begin()) {
#ifdef MAIN_STATE_DEBUG
        Serial.println(F("System: Warning - Failed to initialize backend"));
#endif
        last_jwt_attempt = millis();
        update_state(SystemState::ERROR, "Backend initialization failed");
    } else {
#ifdef MAIN_STATE_DEBUG
        Serial.println(F("System: Successfully initialized backend"));
#endif
        update_state(SystemState::SERVER_CONNECTED, "Backend initialized");
        
        // Verify token validity
        if (!backend.verify_token_validity()) {
#ifdef MAIN_STATE_DEBUG
            Serial.println(F("System: Warning - Token validation failed"));
#endif
            if (!backend.login_jwt()) {
#ifdef MAIN_STATE_DEBUG
                Serial.println(F("System: Warning - Failed to get new JWT token"));
#endif
                update_state(SystemState::ERROR, "JWT token validation failed");
            } else {
                update_state(SystemState::JWT_VALID, "New JWT token obtained");
            }
        } else {
#ifdef MAIN_STATE_DEBUG
            Serial.println(F("System: Token validation successful"));
#endif
            update_state(SystemState::JWT_VALID, "JWT token valid");
        }
    }
  
    // Try to get room ID on startup
    if (try_get_room_id()) {
        update_state(SystemState::ROOM_CONFIGURED, "Room ID obtained");
        // Update room state and send initial state if we have a room ID
        room_state.update_all();
#ifdef MAIN_STATE_DEBUG
        Serial.println(F("System: Sending initial room state on startup."));
#endif
        backend.send_update_room_state(
            static_cast<int>(round(room_state.get_temperature_float())),
            room_state.room_has_activity_bool(),
            static_cast<int>(round(room_state.get_air_quality_float()))
        );
    } else {
        update_state(SystemState::JWT_VALID, "No room ID yet"); // Or SERVER_CONNECTED if JWT also failed but backend.begin() was ok
    }
    last_update = millis();
    last_room_id_attempt = millis();
}

//! Arduino main loop function
void loop() {
    // read sensor data
    room_state.update_all();

    // Only proceed with network operations if we have a valid WiFi connection
    if (current_state < SystemState::WIFI_CONNECTED) {
        return;
    }

    // Try to get JWT token if we don't have one and we're in a state that allows it
    if (!backend.has_token() && 
        (current_state >= SystemState::WIFI_CONNECTED) && 
        // (current_state < SystemState::JWT_VALID) && // Allow retrying if current state is ERROR from JWT fail
        (millis() - last_jwt_attempt > JWT_RETRY_TIME)) {
#ifdef MAIN_STATE_DEBUG
        Serial.println("Attempting to get JWT token...");
#endif
        if (backend.login_jwt()) {
#ifdef MAIN_STATE_DEBUG
            Serial.println("Successfully obtained JWT token");
#endif
            update_state(SystemState::JWT_VALID, "JWT token obtained");
            // After getting token, verify it
            if (backend.verify_token_validity()) {
#ifdef MAIN_STATE_DEBUG
                Serial.println("Token validation successful");
#endif
            } else {
#ifdef MAIN_STATE_DEBUG
                Serial.println("Token validation failed after obtaining new token in loop.");
#endif
                // It might go to ERROR state if verify_token_validity calls make_http_request which fails badly
                // Or m_has_token might be false again from verify_token_validity
            }
        } else {
            update_state(SystemState::ERROR, "Failed to get JWT token in loop");
        }
        last_jwt_attempt = millis();
    }

    // Try to get room ID if we don't have one and we have a valid JWT token
    if (!backend.has_room_id() && 
        (current_state >= SystemState::JWT_VALID) && 
        // (current_state < SystemState::ROOM_CONFIGURED) && // Allow retrying if current state is ERROR from room_id fail
        (millis() - last_room_id_attempt > ROOM_ID_RETRY_TIME)) {
#ifdef MAIN_STATE_DEBUG
        Serial.println("Attempting to get room ID...");
#endif
        if (try_get_room_id()) {
            update_state(SystemState::ROOM_CONFIGURED, "Room ID obtained in loop");
        } else {
            // update_state(SystemState::JWT_VALID, "Failed to get room ID in loop"); // Stay JWT_VALID or could be ERROR
#ifdef MAIN_STATE_DEBUG
            Serial.println("Failed to get room ID in loop.");
#endif
        }
        last_room_id_attempt = millis();
    }

    // send data to server if we have a room ID and are in the correct state
    if (backend.has_room_id() && 
        (current_state == SystemState::ROOM_CONFIGURED) && 
        (millis() - last_update > UPDATE_WAIT_TIME)) {
#ifdef MAIN_STATE_DEBUG
        Serial.println("Sending data...");
#endif
        if (backend.ensure_connection()) {
            last_update = millis();
            if (backend.send_update_room_state(
                static_cast<int>(round(room_state.get_temperature_float())),
                room_state.room_has_activity_bool(),
                static_cast<int>(round(room_state.get_air_quality_float()))
            )) {
                // update_state(SystemState::ROOM_CONFIGURED, "Data sent successfully"); // Already in this state
#ifdef MAIN_STATE_DEBUG
                Serial.println("Data sent successfully.");
#endif
            } else {
                update_state(SystemState::ERROR, "Failed to send data");
            }
        } else {
            update_state(SystemState::ERROR, "Lost server connection before sending data");
        }
    }
}