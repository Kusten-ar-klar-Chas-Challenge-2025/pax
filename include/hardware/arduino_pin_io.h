//! @file arduino_pin_io.h
//! @author Jennifer Gott
//! @date 2025-05-06
//! @brief Arduino pin IO implementation
//! @note This is a singleton class
//! Wraps the Arduino pin IO functions via the PinIO interface

#ifndef HARDWARE_ARDUINO_PIN_IO_H
#define HARDWARE_ARDUINO_PIN_IO_H

#include <Arduino.h> //!< Arduino library
#include "pin_io.h" //!< Pin IO interface
#include "pin_config.h" //!< Pin config
//! @brief Class to wrap Arduino pin IO
//! @details This class is used to wrap the Arduino pin IO functions
class ArduinoPinIO : public PinIO
{
    public:
    //! @brief Call this to setup the configured pins
    //! @details This function will set the mode of all the pins in the pin config array
    //! If the pin config array is not set, the function will not do anything
    void setup() noexcept override
    {
        for (const auto& pin : PinConfig::pins)
        {
            ::pinMode(static_cast<uint8_t>(pin.pin), static_cast<PinMode>(pin.mode));
        }
    }


    //! @brief Read the digital value of a pin
    //! @param pin The pin to read
    //! @return The digital value of the pin
    //! Nodiscard because pin reads should be checked
    [[nodiscard]] bool digitalRead(PinConfig::Pin pin) noexcept override
    {
        return ::digitalRead(static_cast<uint8_t>(pin));
    }


    //! @brief Write the digital value of a pin
    //! @param pin The pin to write to
    //! @param value The value to write to the pin
    void digitalWrite(PinConfig::Pin pin, bool value) noexcept override
    {
        ::digitalWrite(static_cast<uint8_t>(pin), static_cast<PinStatus>(value));
    }


    //! @brief Read the analog value of a pin   
    //! @param pin The pin to read
    //! @return The analog value of the pin
    //! Nodiscard because pin reads should be checked
    [[nodiscard]] uint16_t analogRead(PinConfig::Pin pin) noexcept override
    {
        return ::analogRead(static_cast<uint8_t>(pin));
    }


    //! @brief Write the analog value of a pin
    //! @param pin The pin to write to
    //! @param value The value to write to the pin
    void analogWrite(PinConfig::Pin pin, uint16_t value) noexcept override
    {
        ::analogWrite(static_cast<uint8_t>(pin), value);
    }


    //! @brief Set the mode of a pin
    //! @param pin The pin to set the mode of
    //! @param mode The mode to set the pin to
    void pinMode(PinConfig::Pin pin, uint8_t mode) noexcept override
    {
        ::pinMode(static_cast<uint8_t>(pin), static_cast<PinMode>(mode));
    }


};

#endif
