#ifndef HARDWARE_EEPROM_24LC64_H
#define HARDWARE_EEPROM_24LC64_H

#include "core/interfaces/eeprom.h"
#include "core/interfaces/i2c.h"

//! @brief 24LC64 EEPROM class
//! Concrete implementation of the EEPROM interface for the 24LC64 EEPROM chip
class Eeprom24LC64 : public Eeprom 
{
public:
    //! @brief Constructor for Eeprom24LC64
    //! @param i2c Reference to the I2C interface
    //! @param iic_address The I2C address of the EEPROM (default is 0x50)
    Eeprom24LC64(I2C& i2c, uint8_t iic_address = 0x50) 
        : i2c(i2c), iic_address(iic_address) 
    {
    }

    //! @brief Write data to the EEPROM at specified address
    //! @param memory_address Memory address to write to (0-8191 for 24LC64)
    //! @param[in] data Pointer to data buffer to write from
    //! @param length Number of bytes to write (must not exceed page size)
    //! @return EepromError indicating success or failure type
    //! @note This function enforces page boundaries and accounts for the 5ms write cycle
    //! @warning Writing beyond MAX_ADDRESS or across page boundaries will return an error
    EepromError write(uint16_t memory_address, const uint8_t* data, uint16_t length) override 
    {
        if (data == nullptr) {
            return EepromError::BUFFER_NULLPTR;
        }
        if (memory_address > MAX_ADDRESS || memory_address + length > MEMORY_SIZE) {
            return EepromError::INVALID_ADDRESS;
        }
        if (length > MAX_PAGE_SIZE) {
            return EepromError::DATA_TOO_LONG;
        }
        if (!is_within_page(memory_address, length)) {
            return EepromError::INVALID_ADDRESS; // Page boundary violation
        }

        // Set memory address (big-endian handled by I2C driver)
        if (!i2c.set_address(iic_address, memory_address)) {
            return EepromError::NACK_ADDRESS;
        }

        // Write data
        if (!i2c.write(iic_address, data, length)) {
            return EepromError::WRITE_PROTECTED; // Likely due to write cycle or NACK
        }

        return EepromError::SUCCESS;
    }

    //! @brief Read data from the EEPROM at specified address
    //! @param memory_address Memory address to read from (0-8191 for 24LC64)
    //! @param[out] destination Pointer to buffer where read data will be stored
    //! @param length Number of bytes to read
    //! @return EepromError indicating success or failure type
    //! @note The buffer pointed to by destination must be at least length bytes in size
    //! @warning Reading beyond MAX_ADDRESS will return an error
    EepromError read(uint16_t memory_address, uint8_t* destination, uint16_t length) override 
    {
        if (destination == nullptr) {
            return EepromError::BUFFER_NULLPTR;
        }
        if (memory_address > MAX_ADDRESS || memory_address + length > MEMORY_SIZE) {
            return EepromError::INVALID_ADDRESS;
        }

        // Set memory address (big-endian handled by I2C driver)
        if (!i2c.set_address(iic_address, memory_address)) {
            return EepromError::NACK_ADDRESS;
        }

        // Read data
        if (!i2c.read(iic_address, destination, length)) {
            return EepromError::NACK_DATA; // Read failure (e.g., no response)
        }

        return EepromError::SUCCESS;
    }

    //! @brief Write a C string via a string_view to the EEPROM at specified address
    //! @param memory_address Memory address to write to (0-8191 for 24LC64)
    //! @param[in] source String view containing data to write
    //! @return EepromError indicating success or failure type
    //! @note This writes the raw bytes without null termination
    //! @warning Writing beyond MAX_ADDRESS or across page boundaries will return an error
    EepromError write_string_view(uint16_t memory_address, std::string_view source) override
    {
        if (source.data() == nullptr) {
            return EepromError::BUFFER_NULLPTR;
        }
        if (source.empty()) {
            return EepromError::SUCCESS;
        }

        const uint8_t* data_ptr = reinterpret_cast<const uint8_t*>(source.data());
        uint16_t length = static_cast<uint16_t>(source.length());
        
        return write(memory_address, data_ptr, length);
    }

private:
    static constexpr uint16_t MEMORY_SIZE = 8192;    //!< Total size (8KB, 64kbit)
    static constexpr uint16_t MAX_ADDRESS = 8191;    //!< Max address (0x1FFF)
    static constexpr uint16_t MAX_PAGE_SIZE = 32;    //!< Page buffer size (32 bytes)

    //! @brief Check if a write operation stays within a single page
    //! @param address Starting address of the write
    //! @param length Number of bytes to write
    //! @return True if the operation stays within a single page, false otherwise
    bool is_within_page(uint16_t address, uint16_t length) const {
        uint16_t start_page = address / MAX_PAGE_SIZE;
        uint16_t end_page = (address + length - 1) / MAX_PAGE_SIZE;
        return start_page == end_page;
    }

    I2C& i2c;               //!< Reference to I2C interface
    uint8_t iic_address;    //!< I2C device address
};

#endif // HARDWARE_EEPROM_24LC64_H