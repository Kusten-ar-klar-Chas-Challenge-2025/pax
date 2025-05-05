#ifndef CORE_INTERFACES_SENSOR_MANAGER_H
#define CORE_INTERFACES_SENSOR_MANAGER_H

#include <cstdint>
#include "pin_io.h"
#include "clock_source.h"

struct SensorReading
{
    //! @brief Constructor
    //! @param temperature Temperature in Celsius
    //! @param humidity Humidity in %
    //! @param eco2 CO2 in ppm
    //! @param motion Motion reading from the PIR sensor in binary
    //! @param time_ms Time of reading in milliseconds
    SensorReading(
        float temperature, 
        float humidity, 
        float eco2, 
        bool motion, 
        uint32_t time_ms);
    //! @brief Temperature in Celsius
    float temperature;
    //! @brief Humidity in %
    float humidity;
    //! @brief CO2 in ppm
    float eco2;
    //! @brief Motion in binary
    bool motion;
    //! @brief Time of reading in milliseconds
    uint32_t time_ms;
};

class SensorManager
{
    private:
    //! @brief Latest sensor reading
    SensorReading m_reading = SensorReading(0, 0, 0, false, 0);
    //! @brief PinIO object to read from
    PinIO * m_pin_source;
    //! @brief Clock object to read from
    Clock * m_clock_source;
    protected:
    explicit SensorManager(PinIO * pin_source, Clock * clock_source = nullptr);

    public:
    //! @brief Get the latest sensor reading
    //! @return SensorReading object
    //! @note Should be called after request_reading
    //! @note Returns the latest reading from the sensor
    //! nodiscard because you should use this data or you should not use this function
    [[nodiscard]] virtual SensorReading read() const noexcept = 0;
    //! @brief Request a reading from the sensor
    //! @return Time of request in milliseconds
    //! @note Should be called before read()
    [[nodiscard]] virtual uint32_t request_reading() noexcept = 0;
};

#endif

