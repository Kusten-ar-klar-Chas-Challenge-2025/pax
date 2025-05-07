//! @file i2c.h
//! @brief Interface for I2C operations
//! @author Jennifer Gott
//! @date 2025-05-07

#ifndef INTERFACES_I2C_H
#define INTERFACES_I2C_H

#include <cstdint>

//! @brief Interface for I2C operations
class I2C {
public:
    //! @brief Set the memory address for the next operation
    //! @param device_address The I2C address (7-bit address)
    //! @param memory_address The memory address to use for the next operation
    //! @return true if successful, false otherwise
    virtual bool set_address(uint8_t device_address, uint16_t memory_address) = 0;

    //! @brief Write data to I2C device
    //! @param device_address The I2C device address
    //! @param[in] source The data to write
    //! @param length The length of the data to write
    //! @return true if successful, false otherwise
    virtual bool write(uint8_t device_address, const uint8_t* source, uint16_t length) = 0;

    //! @brief Read data from I2C device
    //! @param device_address The I2C device address
    //! @param[out] destination The buffer to read into
    //! @param length The number of bytes to read
    //! @return true if successful, false otherwise
    virtual bool read(uint8_t device_address, uint8_t* destination, uint16_t length) = 0;

    //! @brief Write a single byte to I2C device
    //! @param device_address The I2C device address
    //! @param[in] source The byte to write
    //! @return true if successful, false otherwise
    virtual bool write_byte(uint8_t device_address, uint8_t source) = 0;

    //! @brief Read a single byte from I2C device
    //! @param device_address The I2C device address
    //! @param[out] destination The byte to read into
    //! @return true if successful, false otherwise
    virtual bool read_byte(uint8_t device_address, uint8_t* destination) = 0;

    virtual ~I2C () = default;
    protected:
    I2C () = default;
};

#endif // INTERFACES_I2C_H 