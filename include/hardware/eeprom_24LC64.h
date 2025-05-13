#ifndef HARDWARE_EEPROM_24LC64_H
#define HARDWARE_EEPROM_24LC64_H

#include <cstdint>
#include <string_view>
#include "clock_source.h"
#include "eeprom_addresses.h"
#include "eeprom.h"
#include "i2c.h"

//! @brief 24LC64 EEPROM class
//! Concrete implementation of the EEPROM interface for the 24LC64 EEPROM chip
class Eeprom24LC64 : public Eeprom 
{
public:
    //! @brief Constructor for Eeprom24LC64
    //! @param i2c Reference to the I2C interface
    //! @param clock Reference to the clock source
    //! @param iic_address The I2C address of the EEPROM (default is 0x50)
    Eeprom24LC64(I2C& i2c, Clock& clock, uint8_t iic_address = 0x50) 
        : i2c(i2c), clock(clock), iic_address(iic_address) 
    {
    }

    //! @brief Write data to the EEPROM at specified address
    //! @param memory_address Memory address to write to (0-8191 for 24LC64)
    //! @param[in] data Pointer to data buffer to write from
    //! @param length Number of bytes to write (must not exceed page size)
    //! @return EepromError indicating success or failure type
    //! @note This function enforces page boundaries and accounts for the 5ms write cycle
    //! @warning Writing beyond the configured max address or across page boundaries will return an error
    EepromError write(uint16_t memory_address, const uint8_t* data, uint16_t length) override 
    {
        if (data == nullptr) {
            return EepromError::BUFFER_NULLPTR;
        }
        if (!EepromAddresses::is_valid(memory_address) || !EepromAddresses::is_valid(memory_address + length)) {
            return EepromError::INVALID_ADDRESS;
        }
        if (length > EepromAddresses::PageSize) {
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
    //! @warning Reading beyond the configured max address will return an error
    EepromError read(uint16_t memory_address, uint8_t* destination, uint16_t length) override 
    {
        if (destination == nullptr) {
            return EepromError::BUFFER_NULLPTR;
        }
        if (!EepromAddresses::is_valid(memory_address) || !EepromAddresses::is_valid(memory_address + length)) {
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
    //! @warning Writing beyond the configured max address or across page boundaries will return an error
    EepromError write(uint16_t memory_address, std::string_view source) override
    {
        if (source.data() == nullptr) {
            return EepromError::BUFFER_NULLPTR;
        }
        if (source.empty()) {
            return EepromError::SUCCESS;
        }

        if (!EepromAddresses::is_valid(memory_address) || !EepromAddresses::is_valid(memory_address + source.length())) {
            return EepromError::INVALID_ADDRESS;
        }

        if (!EepromAddresses::is_page_aligned(memory_address)) {
            return EepromError::INVALID_ADDRESS;
        }

        const uint8_t* data_ptr = reinterpret_cast<const uint8_t*>(source.data());
        uint16_t length = static_cast<uint16_t>(source.length());
        
        return write(memory_address, data_ptr, length);
    }

    //! @brief Write a string view to the EEPROM across multiple pages
    //! @param memory_address Memory address to write to (0-8191 for 24LC64)
    //! @param[in] source String view containing data to write
    //! @return EepromError indicating success or failure type
    //! @note This function handles writing across page boundaries by calling write() for each page
    //! @note You, the caller, must make sure that you are not overwriting important data by checking the address and length
    //! @warning Writing beyond the configured max address will return an error
    EepromError write_across_pages(uint16_t memory_address, std::string_view source)
    {
        if (source.data() == nullptr) {
            return EepromError::BUFFER_NULLPTR;
        }
        if (source.empty()) {
            return EepromError::SUCCESS;
        }

        if (!EepromAddresses::is_valid(memory_address) || !EepromAddresses::is_valid(memory_address + source.length())) {
            return EepromError::INVALID_ADDRESS;
        }

        // Calculate number of pages needed
        size_t total_bytes = source.length();
        size_t current_page = memory_address / EepromAddresses::PageSize;
        size_t end_page = (memory_address + total_bytes - 1) / EepromAddresses::PageSize;
        size_t bytes_written = 0;

        // Write each page
        for (size_t page = current_page; page <= end_page; ++page) {
            uint16_t page_start = page * EepromAddresses::PageSize;
            uint16_t page_end = page_start + EepromAddresses::PageSize;
            
            // Calculate bytes to write in this page
            uint16_t page_offset = (page == current_page) ? (memory_address % EepromAddresses::PageSize) : 0;
            uint16_t bytes_in_page = std::min(
                static_cast<uint16_t>(EepromAddresses::PageSize - page_offset),
                static_cast<uint16_t>(total_bytes - bytes_written)
            );

            // Write the page
            EepromError result = write(
                page_start + page_offset,
                source.substr(bytes_written, bytes_in_page)
            );
            
            if (result != EepromError::SUCCESS) {
                return result;
            }

            bytes_written += bytes_in_page;
            clock.delay(5); // Wait for write cycle to complete
        }   

        return EepromError::SUCCESS;
    }

private:

    //! @brief Check if a write operation stays within a single page
    //! @param address Starting address of the write
    //! @param length Number of bytes to write
    //! @return True if the operation stays within a single page, false otherwise
    bool is_within_page(uint16_t address, uint16_t length) const {
        uint16_t start_page = address / EepromAddresses::PageSize;
        uint16_t end_page = (address + length - 1) / EepromAddresses::PageSize;
        return start_page == end_page;
    }

    I2C& i2c;               //!< Reference to I2C interface
    Clock& clock;            //!< Reference to the clock source
    uint8_t iic_address;    //!< I2C device address
};

#endif // HARDWARE_EEPROM_24LC64_H