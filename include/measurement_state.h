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
* @class MeasurementState 
* @brief Class to handle sensor measurements over time
* 
* @details It it written for an Arduino UNO R4
* Sensors implemented: PIR Sensor
*/
class MeasurementState {
private:

    //! The Arduino Pin where the PIR Sensor is connected
    uint8_t m_pirPin;
    
    //! Time in milliseconds the class will hold last positive reading
    unsigned long m_holdDuration;

    //! The last time a positive reading was made
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
     * @param pirPin determines I/O pin for reading
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
     * 
     * @return true if room has found activity 
     * @return false 
     */
    bool roomHasActivity();
};



#endif