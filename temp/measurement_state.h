/**
 * @file MeasurementState.h
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief 
 * @version 0.1
 * @date 2025-04-07
 * 
 */
#ifndef PAX_MEASUREMENT_STATE_CLASS
#define PAX_MEASUREMENT_STATE_CLASS

#include <Arduino.h>

/**
 * @brief Class to handle sensor measurements over time
 * 
 * It it written for an Arduino UNO R4
 * Sensors implemented: PIR Sensor
 */
class MeasurementState {
private:
    /**
     * @brief The Arduino Pin where the PIR Sensor is connected
     */
    uint8_t m_pirPin;
    /**
     * @brief Time in milliseconds the class will hold last positive reading
     */
    unsigned long m_holdDuration;
    /**
     * @brief The last time a positive reading was made
     */
    unsigned long m_lastActivationTime;
    /**
     * @brief A wrapper around Arduino function millis()
     * 
     * @return unsigned long 
     */
    unsigned long getCurrentTime();
public:
    /**
     * @brief Constructor
     * 
     * @param pirPin 
     * @param holdDuration in milliseconds
     */
    MeasurementState(uint8_t pirPin, unsigned long holdDuration = 5 * 1 * 1000);
    /**
     * @brief Run in setup to set correct pinMode
     * 
     */
    void init();
    /**
     * @brief Read sensor value and update private variables
     * 
     */
    void update();
    /**
     * @brief Function to check current room status
     */
    bool roomHasActivity();
};



#endif