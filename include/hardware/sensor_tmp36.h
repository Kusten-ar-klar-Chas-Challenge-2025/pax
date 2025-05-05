#ifndef HARDWARE_SENSOR_TMP36_H
#define HARDWARE_SENSOR_TMP36_H

#include "pin_config.h"
//! PinConfig::pin_temp_sensor
#include <Arduino.h>

class TempSensor {
    public:
    [[nodiscard]] static float read_celsius () noexcept;
    static bool ready_for_reading();
    static void setup(uint8_t pin = PinConfig::pin_temp_sensor);
};

//! TemperatureSensor::read_celsius

#endif