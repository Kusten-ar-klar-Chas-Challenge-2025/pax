#ifndef HARDWARE_ARDUINO_PIN_IO_H
#define HARDWARE_ARDUINO_PIN_IO_H

#include "pin_io.h"

//! @brief Class to wrap Arduino pin IO
//! @details This class is used to wrap the Arduino pin IO functions
class ArduinoPinIO : public PinIO
{
    public:
    //! @brief Call this to setup the configured pins
    //! @details This function will set the mode of all the pins in the pin config array
    //! If the pin config array is not set, the function will not do anything
    void setup() noexcept override;
    //! @brief Read the digital value of a pin
    //! @param pin The pin to read
    //! @return The digital value of the pin
    //! Nodiscard because pin reads should be checked
    [[nodiscard]] bool digitalRead(uint8_t pin) noexcept override;
    //! @brief Write the digital value of a pin
    //! @param pin The pin to write to
    //! @param value The value to write to the pin
    void digitalWrite(uint8_t pin, bool value) noexcept override;
    //! @brief Read the analog value of a pin   
    //! @param pin The pin to read
    //! @return The analog value of the pin
    //! Nodiscard because pin reads should be checked
    [[nodiscard]] uint16_t analogRead(uint8_t pin) noexcept override;
    //! @brief Write the analog value of a pin
    //! @param pin The pin to write to
    //! @param value The value to write to the pin
    void analogWrite(uint8_t pin, uint16_t value) noexcept override;
    //! @brief Set the mode of a pin
    //! @param pin The pin to set the mode of
    //! @param mode The mode to set the pin to
    void pinMode(uint8_t pin, uint8_t mode) noexcept override;
};

#endif
