/**
 * @file temp_sensor.h
 * @brief This class handles the temperature and humidity sensor DHT11
 * 
 */
#ifndef TEMP_SENSOR_H
#define TEMP_SENSOR_H
#include <Arduino.h>

/**
 * @class Class for temperature and humidity sensor DHT11
 * 
 */
class TempSensor
{
public:
    //! Constructor for temp sensor.
    TempSensor(uint8_t pin, float temperature_offset = 0);

    /**
     * @brief Initialize sensor
     */
    void begin();

    /**
     * @brief Get the last read temperature value
     * @return float value of temperature (°C)
     */
    float get_temperature();

    /**
     * @details Humidity might be used to calibrate CO2 sensor
     * @return float value of humidity (%)
     */
    float get_humidity();

    /**
     * @brief A method for getting the current time passed (ms)
     * @return millis()
     */
    unsigned long current_time();

    /**
     * @brief Set a new temperature offset
     * 
     * @param new_temperature_offset (new offset, accepts values between -50 and 50 °C)
     * @return on success return true
     */
    bool set_temperature_offset(float new_temperature_offset);
    /**
     * @brief Get current temperature offset
     * 
     * @return float 
     */
    float get_temperature_offset();

private:
    /**
     * @brief Populate byte array
     */
    void store_sensor();

    /**
     * @brief Method will store data in byte array
     * @return Arduino define byte array
     */
    byte read_sensor();

    //! Store last polled time with millis()
    unsigned long last_polled;
    //! Array to store data and read from
    byte data[5];
    //! Pin number of DHT 11 sensor.
    uint8_t m_pin;
    float m_temperature_offset;
};

/*
Example Usage:
#include "temp_sensor.h"
    TempSensor DHT11(PIN_NUMBER);

    void setup()
    {
        DHT11.begin();
    }
    void loop(
    {
        DHT11.get_temperature();
    }
*/

#endif // TEMP_SENSOR