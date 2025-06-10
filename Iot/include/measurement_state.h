/**
 * @file MeasurementState.h
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief 
 * @version 0.2
 * @date 2025-05-19
 * 
 */
#ifndef PAX_MEASUREMENT_STATE_CLASS
#define PAX_MEASUREMENT_STATE_CLASS

#include <Arduino.h>
#include <Adafruit_SGP30.h>
#include <I2C_eeprom.h>
#include "temp_sensor.h"


/**
 * @brief Class to handle sensor measurements over time for Arduino UNO R4
 * 
 * Sensors implemented: PIR Sensor
 */
class MeasurementState {
private:
    /**
     * @brief The Arduino Pin where the PIR Sensor is connected
     */
    uint8_t m_pir_pin;
    /**
     * @brief Time in milliseconds the class will hold last positive reading
     */
    unsigned long m_hold_duration;
    /**
     * @brief The last time a positive reading was made
     */
    unsigned long m_last_activation_time;
    /**
     * @brief A wrapper around Arduino function millis()
     * 
     * @return unsigned long 
     */
    unsigned long get_current_time();
    /**
     * @brief Internal SGP30 sensor object
     * 
     */
    Adafruit_SGP30 m_sgp;
    /**
     * @brief The latest cached Air Quality reading from SGP30
     * defaults to 50%
     * 
     */
    float m_air_quality { 50.0f };
    /**
     * @brief Internal variable indicating if SGP30 is properly initalized
     * 
     */
    bool m_sgp_initialized { false };
    /**
     * @brief Internal Temperature Sensor object
     * 
     */
    TempSensor m_temp_sensor;
    /**
     * @brief The latest cached Temperature reading
     * 
     */
    float m_temperature { 22.0f };
    /**
     * @brief The latest cached Humidity reading
     * 
     */
    float m_humidity { 50.0f };
    /**
     * @brief Internal variable indicating if a Temperature Sensor is used
     * 
     */
    bool m_temp_sensor_connected {};
    /**
     * @brief Calibration varible for SGP30 sensor
     * 
     */
    uint16_t m_iaq_baseline_eco2;
    /**
     * @brief Calibration varible for SGP30 sensor
     * 
     */
    uint16_t m_iaq_baseline_tvoc;
    
    I2C_eeprom* m_eeprom { nullptr };
    /**
     * @brief Read SGP30 values and update internal variables
     * 
     */
    void read_air_quality();
    /**
     * @brief Reads messages from serial and if command is valid updates offset parameter
     * 
     * Example commands: 
     * TEMP0        resets offset
     * TEMP+1       sets offset to +1.0
     * TEMP-1.5     set offset to -1.5
     * 
     * @param offset reference is updated if a valid command was received
     * @return true if a valid command was received
     */
    bool get_new_temperature_offset_from_serial(float& offset);
    bool writeFloatToEEPROM(uint16_t eeprom_addr, size_t buffer_index, float value);
    bool readFloatFromEEPROM(uint16_t eeprom_addr, size_t buffer_index, float* destination);

public:
    /**
     * @brief Constructor
     * 
     * @param pir_pin 
     * @param hold_duration in milliseconds
     * @param temp_sensor_pin if not passed getTemperature will give error message
     */
    MeasurementState(uint8_t pir_pin, unsigned long hold_duration = 5 * 1 * 1000, uint8_t temp_sensor_pin = 0);
    /**
     * @brief Run in setup to set correct pinMode
     * 
     */
    void begin(I2C_eeprom* eeprom);
    /**
     * @brief 
     * 
     * @param address EEPROM can store values at addresses between 0 and 7999
     * @return true if successful
     */
    bool update_temperature_offset_from_eeprom(uint16_t address);
    /**
     * @brief Reads from serial, and if command is valid updates temperature offset 
     * 
     * @return true if a new offset was successfully set
     */
    bool update_temperature_offset_from_serial(uint16_t eeprom_address);
    /**
     * @brief Set new temperature offset for TempSensor
     * 
     * @param new_temperature_offset (Accepts values between -50 and 50)
     * @return true on success
     */
    bool set_temperature_offset(float new_temperature_offset);
    /**
     * @brief Get current temperature offset
     * 
     * @return float 
     */
    float get_temperature_offset();
    /**
     * @brief Read PIR sensor value for room activity and update private variables
     * 
     */
    void update_pir();
    /**
     * @brief Read PIR sensor value for room activity and temperature and co2 sensors
     * and update private variables
     * 
     */
    void update_all();
    /**
     * @brief Function to show current room status in a printable format
     * 
     * @return String boolean, "true" or "false"
     */
    String room_has_activity();
    /**
     * @brief Function returns current room activity status
     * 
     * @return bool true if room has activity, false otherwise
     */
    bool room_has_activity_bool();
    /**
     * @brief Function returns Air Quality in a printable format 
     * 
     * @return String Values 0-100, representing percentage 
     */
    String get_air_quality();
    /**
     * @brief Function returns Air Quality as a float
     * 
     * @return float Values 0-100, representing percentage
     */
    float get_air_quality_float();
    /**
     * @brief Function returns Temperature in a printable format
     * 
     * @return String temperature in °C
     */
    String get_temperature();
    /**
     * @brief Function returns Temperature as a float
     * 
     * @return float temperature in °C
     */
    float get_temperature_float();
};

#endif