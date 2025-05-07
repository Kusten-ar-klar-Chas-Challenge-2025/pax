#ifndef MOCK_I2C_H
#define MOCK_I2C_H

#include "i2c.h"
#include <vector>
#include <algorithm>
#include <chrono>

//! @brief Mock I2C class implementing the I2C interface for testing EEPROM operations
//! @author Jennifer Gott
//! @date 2025-05-07
class MockI2C : public I2C {

public:
    //! @brief Constructor initializing the mock I2C memory
    MockI2C() : memory(MEMORY_SIZE, 0), current_address(0), address_set(false), write_in_progress(false) {}

    //! @brief Set the memory address pointer for subsequent read or write operations
    //! @param device_address The I2C device address (ignored in mock, used for compatibility)
    //! @param memory_address The memory address to set
    //! @return true if successful, false otherwise
    bool set_address(uint8_t /*device_address*/, uint16_t memory_address) override {
        if (memory_address >= MEMORY_SIZE) {
            return false;
        }
        current_address = memory_address;
        address_set = true;
        return true;
    }

    //! @brief Write data to the mock I2C device at the current address pointer
    //! @param device_address The I2C device address (ignored in mock, used for compatibility)
    //! @param source The data to write
    //! @param length The length of the data to write
    //! @return true if successful, false otherwise
    bool write(uint8_t /*device_address*/, const uint8_t* source, uint16_t length) override {
        // Check if write is allowed and address is set
        if (!is_write_allowed() || !address_set) {
            return false;
        }

        // Validate inputs
        if (source == nullptr || length == 0) {
            return false;
        }
        if (current_address + length > MEMORY_SIZE) {
            return false;
        }
        if (!is_within_page(current_address, length)) {
            return false; // Page boundary violation
        }

        // Copy data to memory
        std::copy(source, source + length, memory.begin() + current_address);
        current_address += length; // Increment address pointer

        // Update write cycle state
        last_write = std::chrono::steady_clock::now();
        write_in_progress = true;
        
        return true;
    }

    //! @brief Read data from the mock I2C device at the current address pointer
    //! @param device_address The I2C device address (ignored in mock, used for compatibility)
    //! @param destination The buffer to read into
    //! @param length The number of bytes to read
    //! @return true if successful, false otherwise
    bool read(uint8_t /*device_address*/, uint8_t* destination, uint16_t length) override {
        // Validate inputs and address
        if (destination == nullptr || length == 0 || !address_set) {
            return false;
        }
        if (current_address + length > MEMORY_SIZE) {
            return false;
        }

        // Copy data from memory
        std::copy(memory.begin() + current_address, memory.begin() + current_address + length, destination);
        current_address += length; // Increment address pointer
        
        return true;
    }

    //! @brief Write a single byte to the mock I2C device at the current address pointer
    //! @param device_address The I2C device address (ignored in mock, used for compatibility)
    //! @param source The byte to write
    //! @return true if successful, false otherwise
    bool write_byte(uint8_t /*device_address*/, uint8_t source) override {
        // Check if write is allowed and address is set
        if (!is_write_allowed() || !address_set) {
            return false;
        }
        if (current_address >= MEMORY_SIZE) {
            return false;
        }

        // Write single byte
        memory[current_address] = source;
        current_address++; // Increment address pointer

        // Update write cycle state
        last_write = std::chrono::steady_clock::now();
        write_in_progress = true;
        
        return true;
    }

    //! @brief Read a single byte from the mock I2C device at the current address pointer
    //! @param device_address The I2C device address (ignored in mock, used for compatibility)
    //! @param destination The byte to read into
    //! @return true if successful, false otherwise
    bool read_byte(uint8_t /*device_address*/, uint8_t* destination) override {
        // Validate inputs and address
        if (destination == nullptr || !address_set || current_address >= MEMORY_SIZE) {
            return false;
        }

        // Read single byte
        *destination = memory[current_address];
        current_address++; // Increment address pointer
        
        return true;
    }
    private:
    static constexpr uint16_t MEMORY_SIZE = 8192;        //!< Total simulated EEPROM size (8KB, 64kbit)
    static constexpr uint16_t PAGE_SIZE = 32;            //!< Page buffer size (32 bytes)
    static constexpr uint32_t WRITE_CYCLE_MS = 5;        //!< Write cycle time (5ms)
    
    std::vector<uint8_t> memory;                         //!< Simulated EEPROM memory buffer
    uint16_t current_address;                            //!< Current memory address pointer
    bool address_set;                                    //!< Flag indicating if address is set
    std::chrono::steady_clock::time_point last_write;    //!< Timestamp of the last write operation
    bool write_in_progress;                              //!< Flag to track write cycle status
    
    //! @brief Check if a write operation stays within a single page buffer
    //! @param address Starting address of the write
    //! @param length Number of bytes to write
    //! @return True if the operation stays within a single page, false otherwise
    bool is_within_page(uint16_t address, uint16_t length) const {
        uint16_t start_page = address / PAGE_SIZE;
        uint16_t end_page = (address + length - 1) / PAGE_SIZE;
        return start_page == end_page;
    }

    //! @brief Check if a write operation can proceed (not in write cycle)
    //! @return True if enough time has passed since last write, false otherwise
    //! @author Jennifer Gott
    //! @date 2025-05-07
    bool is_write_allowed() {
        if (!write_in_progress) {
            return true;
        }
        auto now = std::chrono::steady_clock::now();
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now - last_write).count();
        if (elapsed_ms >= WRITE_CYCLE_MS) {
            write_in_progress = false;
            return true;
        }
        return false;
    }

};

#endif // MOCK_I2C_H