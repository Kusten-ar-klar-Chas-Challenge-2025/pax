#ifndef MOCK_IO_H
#define MOCK_IO_H

#include "pin_io.h"
#include <map>
#include <random>
#include "pin_config.h"

//! @brief Mock class for PinIO
//! @details Mock class for PinIO to be used in unit tests
class MockPinIO : public PinIO
{
    private:
    std::map<uint8_t, bool> m_digitalReadMap;
    std::map<uint8_t, uint16_t> m_analogReadMap;
    uint16_t m_analog_resolution {1023};
    std::random_device rd;
    std::mt19937 gen;

    public:

    MockPinIO() noexcept : m_digitalReadMap(), m_analogReadMap(), m_analog_resolution(1023), rd(), gen(rd()) {}
    //! @brief Constructor
    //! @param m_digitalReadMap Map of digital read values
    //! @param m_analogReadMap Map of analog read values
    MockPinIO(std::map<uint8_t, bool> m_digitalReadMap, std::map<uint8_t, uint16_t> m_analogReadMap) noexcept :
        m_digitalReadMap(m_digitalReadMap),
        m_analogReadMap(m_analogReadMap)
    {
        gen = std::mt19937(rd());
    }

    void setup() noexcept override {
        m_digitalReadMap.clear();
        m_analogReadMap.clear();
    }
    
    //! @brief Returns values set in the map, to operate as a mock  
    //! @param pin The pin to get the value for
    //! @return The value set in the map
    //! Nodiscard because pin reads should be checked
    [[nodiscard]] bool digitalRead(PinConfig::Pin pin) noexcept override
    {
        if (m_digitalReadMap.find(static_cast<uint8_t>(pin)) == m_digitalReadMap.end())
        {

            std::uniform_int_distribution<> dis(0, m_analog_resolution);
            return dis(gen);
        }
        return m_digitalReadMap[static_cast<uint8_t>(pin)];
    }

    //! @brief Returns values set in the map, to operate as a mock
    //! @param pin The pin to get the value for
    //! @return The value set in the map
    //! Nodiscard because pin reads should be checked
    [[nodiscard]] uint16_t analogRead(PinConfig::Pin pin) noexcept override
    {
        if (m_analogReadMap.find(static_cast<uint8_t>(pin)) == m_analogReadMap.end())
        {
            // return a random value
            return std::rand() % m_analog_resolution;
        }
        return m_analogReadMap[static_cast<uint8_t>(pin)];
    }
    
    //! @brief Sets values in the map, to operate as a mock 
    //! @param pin The pin to set the value for
    //! @param value The value to set
    //! @note Should be true or false
    void digitalWrite(PinConfig::Pin pin, bool value) noexcept override
    {
        if (m_digitalReadMap.find(static_cast<uint8_t>(pin)) == m_digitalReadMap.end())
        {
            return;
        }
        m_digitalReadMap[static_cast<uint8_t>(pin)] = value;
    }

    //! @brief Sets values in the map, to operate as a mock
    //! @param pin The pin to set the value for
    //! @param value The value to set
    //! @note Should be 0 to 1023 for 10 bit resolution or 0 to 4095 for 12 bit resolution
    void analogWrite(PinConfig::Pin pin, uint16_t value) noexcept override
    {
        if (value > m_analog_resolution)
        {
            value = m_analog_resolution;
        }
        m_analogReadMap[static_cast<uint8_t>(pin)] = value;
    }

    //! @brief Sets values in the map, for setting state in the test
    //! @param pin The pin to set the value for
    //! @param value The value to set
    //! @note Should be true or false
    void setDigitalRead(PinConfig::Pin pin, bool value) noexcept
    {
        if (m_digitalReadMap.find(static_cast<uint8_t>(pin)) == m_digitalReadMap.end())
        {
            return;
        }
        m_digitalReadMap[static_cast<uint8_t>(pin)] = value;
    }

    //! @brief Sets values in the map, for setting state in the test
    //! @param pin The pin to set the value for
    //! @param value The value to set
    //! @note Should be 0 to 1023 for 10 bit resolution or 0 to 4095 for 12 bit resolution
    void setAnalogRead(PinConfig::Pin pin, uint16_t value) noexcept
    {
        if (value > m_analog_resolution)
        {
            value = m_analog_resolution;
        }
        m_analogReadMap[static_cast<uint8_t>(pin)] = value;
    }

    void pinMode(PinConfig::Pin pin, uint8_t mode) noexcept override {
        return;
    }
    
    //! @brief Sets the analog resolution
    //! @param resolution The resolution to set
    //! For setting the max value for the analogRead
    //! @note Should be 1023 for 10 bit resolution or 4095 for 12 bit resolution
    void set_analog_resolution(uint16_t resolution) noexcept
    {
        if (resolution > 1023)
        {
            m_analog_resolution = 4095;
        }
        else
        {
            m_analog_resolution = 1023;
        }
    }
};

#endif  // MOCK_IO_H