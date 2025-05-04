#include "sensor_manager.h"

SensorManager::SensorManager(PinIO * pin_source, Clock * clock_source)
    : m_pin_source(pin_source), m_clock_source(clock_source)
{
}

SensorReading SensorManager::read() const noexcept
{
    return m_reading;
}

uint32_t SensorManager::request_reading() noexcept
{
    //! functions to read from the sensor to m_reading
    return m_clock_source->get_time_ms();
}