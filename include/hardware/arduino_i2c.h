#ifndef HARDWARE_ARDUINO_I2C_H
#define HARDWARE_ARDUINO_I2C_H

#include "core/interfaces/i2c.h"
#include <Wire.h>

//! @brief Arduino-specific I2C implementation using Wire library
class ArduinoI2C : public I2C {
public:
    //! @brief Constructor
    //! @param wire Reference to the TwoWire instance (e.g., Wire or Wire1)
    ArduinoI2C(TwoWire& wire) : wire(wire) {}

    //! @brief Sets the memory address of the I2C device we are going to use
    //! @param device_address The device address of the I2C device (7-bit)
    //! @param memory_address The memory address of the I2C device
    //! @return true if successful, false otherwise
    bool set_address(uint8_t device_address, uint16_t memory_address) {
        wire.beginTransmission(device_address);
        wire.write(memory_address);
        return wire.endTransmission() == 0;
    }

    //! @brief Write data to I2C device
    //! @param address The I2C device address
    //! @param source The data to write
    //! @param length The length of the data to write
    //! @return true if successful, false otherwise
    bool write(uint8_t address, const uint8_t* source, uint16_t length) override {
        wire.beginTransmission(address);
        size_t bytesWritten = wire.write(source, length);
        uint8_t result = wire.endTransmission();
        return bytesWritten == length && result == 0;
    }

    //! @brief Read data from I2C device
    //! @param address The I2C device address
    //! @param destination The buffer to read into
    //! @param length The length of the data to read
    //! @return true if successful, false otherwise
    bool read(uint8_t address, uint8_t* destination, uint16_t length) override {
        uint8_t bytesRead = wire.requestFrom(address, length);
        if (bytesRead != length) return false;

        for (uint16_t i = 0; i < length; i++) {
            if (!wire.available()) return false;
            destination[i] = wire.read();
        }
        return true;
    }

    //! @brief Write a single byte to I2C device
    //! @param address The I2C device address
    //! @param source The byte to write
    //! @return true if successful, false otherwise
    bool write_byte(uint8_t address, uint8_t source) override {
        wire.beginTransmission(address);
        size_t bytesWritten = wire.write(source);
        uint8_t result = wire.endTransmission();
        return bytesWritten == 1 && result == 0;
    }

    //! @brief Read a single byte from I2C device
    //! @param address The I2C device address
    //! @param destination The byte to read into
    //! @return true if successful, false otherwise
    bool read_byte(uint8_t address, uint8_t* destination) override {
        uint8_t bytesRead = wire.requestFrom(address, static_cast<uint8_t>(1));
        if (bytesRead != 1) return false;
        *destination = wire.read();
        return true;
    }

private:
    TwoWire& wire; //!< Reference to the TwoWire instance
};

#endif // HARDWARE_ARDUINO_I2C_H 