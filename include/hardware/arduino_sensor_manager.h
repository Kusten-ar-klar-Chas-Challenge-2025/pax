#ifndef HARDWARE_ARDUINO_SENSOR_MANAGER_H
#define HARDWARE_ARDUINO_SENSOR_MANAGER_H

#include "sensor_manager.h"

class TempSensor {
    public:
    [[nodiscard]] static float read_celsius () noexcept;
    static bool ready_for_reading();
    static void setup(uint8_t pin);
};

//! TemperatureSensor::read_celsius


class ArduinoSensorManager : public SensorManager
{
    private:
    //! SensorReading m_reading; //senaste läsningen
    SensorReading m_next;

    public:
    ArduinoSensorManager(PinIO * pin_source, Clock * clock_source = nullptr);
    SensorReading read () const noexcept override;
    uint32_t request_reading() noexcept;

};

#endif