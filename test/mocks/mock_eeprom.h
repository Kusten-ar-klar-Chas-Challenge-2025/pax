#ifndef MOCK_EEPROM_H
#define MOCK_EEPROM_H

#include "eeprom.h"
#include <vector>
#include <algorithm>
#include <chrono>

//! @brief Mock EEPROM class implementing the Eeprom interface with page buffer and write cycle timing
class MockEeprom : public Eeprom {
private:
    static constexpr uint16_t MEMORY_SIZE = 8192;        //!< Total simulated EEPROM size (8KB, 64kbit)
    static constexpr uint16_t PAGE_SIZE = 32;            //!< Page buffer size (32 bytes)
    static constexpr uint32_t WRITE_CYCLE_MS = 5;        //!< Write cycle time (5ms)
    
    std::vector<uint8_t> memory;                         //!< Simulated EEPROM memory buffer
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

public:
    //! @brief Constructor initializing the mock EEPROM memory
    MockEeprom() : memory(MEMORY_SIZE, 0), write_in_progress(false) {}

    //! @brief Write data to the mock EEPROM
    //! @param address The starting address to write to
    //! @param source Pointer to the source data to write
    //! @param length The number of bytes to write
    //! @return EepromError indicating success or specific error condition
    EepromError write(uint16_t address, const uint8_t* source, uint16_t length) override {
        // Check if write is allowed
        if (!is_write_allowed()) {
            return EepromError::WRITE_PROTECTED;
        }

        // Check for invalid inputs
        if (source == nullptr) {
            return EepromError::BUFFER_NULLPTR;
        }
        if (address >= MEMORY_SIZE || address + length > MEMORY_SIZE) {
            return EepromError::INVALID_ADDRESS;
        }
        if (!is_within_page(address, length)) {
            return EepromError::INVALID_ADDRESS; // Simulate page buffer boundary violation
        }

        // Simulate write operation
        std::copy(source, source + length, memory.begin() + address);
        
        // Update write cycle state
        last_write = std::chrono::steady_clock::now();
        write_in_progress = true;
        
        return EepromError::SUCCESS;
    }

    //! @brief Read data from the mock EEPROM
    //! @param address The starting address to read from
    //! @param destination Pointer to the buffer to store read data
    //! @param length The number of bytes to read
    //! @return EepromError indicating success or specific error condition
    EepromError read(uint16_t address, uint8_t* destination, uint16_t length) override {
        // Check for invalid inputs
        if (destination == nullptr) {
            return EepromError::BUFFER_NULLPTR;
        }
        if (address >= MEMORY_SIZE || address + length > MEMORY_SIZE) {
            return EepromError::INVALID_ADDRESS;
        }

        // Simulate read operation
        std::copy(memory.begin() + address, memory.begin() + address + length, destination);
        return EepromError::SUCCESS;
    }

    //! @brief Write a string view to the mock EEPROM
    //! @param address The starting address to write to
    //! @param source The string view containing the data to write
    //! @return EepromError indicating success or specific error condition
    EepromError write_string_view(uint16_t address, std::string_view source) override {
        // Check if write is allowed
        if (!is_write_allowed()) {
            return EepromError::WRITE_PROTECTED;
        }

        // Check for invalid address
        if (address >= MEMORY_SIZE || address + source.size() > MEMORY_SIZE) {
            return EepromError::INVALID_ADDRESS;
        }
        if (!is_within_page(address, source.size())) {
            return EepromError::INVALID_ADDRESS; // Simulate page buffer boundary violation
        }

        // Simulate write operation
        std::copy(source.begin(), source.end(), memory.begin() + address);
        
        // Update write cycle state
        last_write = std::chrono::steady_clock::now();
        write_in_progress = true;
        
        return EepromError::SUCCESS;
    }
};

#endif // MOCK_EEPROM_H