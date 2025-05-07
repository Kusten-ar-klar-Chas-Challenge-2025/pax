#ifndef EEPROM_DEVICE_ID_H
#define EEPROM_DEVICE_ID_H

#include "core/interfaces/eeprom.h"
#include <cstring>
#include <algorithm>

//! @brief Device ID format for storage in 24LC64 EEPROM (28 bytes)
struct DeviceId {
    char prefix[4];        //!< ASCII prefix "DID:" (4 bytes)
    uint8_t version;       //!< Format version (1 byte)
    uint8_t unique_id[16]; //!< 128-bit UUID (16 bytes)
    uint8_t reserved[5];   //!< Reserved for future use (5 bytes)
    uint16_t checksum;     //!< CRC16 checksum (2 bytes)

    static constexpr size_t SIZE = 28; //!< Total size in bytes

    //! @brief Initialize the device ID with a UUID
    DeviceId(const uint8_t* uuid) {
        std::memcpy(prefix, "DID:", 4);
        version = 0x01;
        std::memcpy(unique_id, uuid, 16);
        std::memset(reserved, 0, 5);
        checksum = compute_crc16();
    }

    //! @brief Default constructor for reading from EEPROM
    DeviceId() = default;

    //! @brief Compute CRC16-CCITT checksum of the device ID
    uint16_t compute_crc16() const {
        uint16_t crc = 0xFFFF;
        const uint8_t* data = reinterpret_cast<const uint8_t*>(this);
        for (size_t i = 0; i < SIZE - 2; ++i) { // Exclude checksum itself
            crc ^= (data[i] << 8);
            for (int j = 0; j < 8; ++j) {
                if (crc & 0x8000) {
                    crc = (crc << 1) ^ 0x1021;
                } else {
                    crc <<= 1;
                }
            }
        }
        return crc;
    }

    //! @brief Verify the device ID checksum
    bool is_valid() const {
        return checksum == compute_crc16();
    }
};

//! @brief Manages a DeviceId in EEPROM, reading on construction and writing on destruction if needed
class EepromDeviceId {
public:
    //! @brief Constructor: Read DeviceId from EEPROM or initialize with provided UUID
    //! @param uuid 16-byte UUID to use if EEPROM has no valid DeviceId
    //! @param eeprom Reference to EEPROM interface
    //! @param memory_address Page-aligned address to store/read DeviceId
    EepromDeviceId(const uint8_t* uuid, Eeprom& eeprom, uint16_t memory_address)
        : eeprom(eeprom), memory_address(memory_address) {
        // Validate address
        if (memory_address + DeviceId::SIZE > 8192 || // Beyond EEPROM size
            !is_page_aligned(memory_address)) {       // Not page-aligned
            device_id = DeviceId(uuid); // Fall back to provided UUID
            return;
        }

        // Try to read existing DeviceId
        DeviceId read_id;
        EepromError result = eeprom.read(memory_address, reinterpret_cast<uint8_t*>(&read_id), DeviceId::SIZE);
        if (result == EepromError::SUCCESS && read_id.is_valid()) {
            device_id = read_id; // Use existing DeviceId
        } else {
            device_id = DeviceId(uuid); // Initialize with provided UUID
        }
    }

    //! @brief Destructor: Write DeviceId to EEPROM if no valid ID exists
    ~EepromDeviceId() {
        // Check if EEPROM has a valid DeviceId
        DeviceId read_id;
        EepromError result = eeprom.read(memory_address, reinterpret_cast<uint8_t*>(&read_id), DeviceId::SIZE);
        if (result == EepromError::SUCCESS && read_id.is_valid()) {
            return; // Valid ID exists, no need to write
        }

        // Write DeviceId if address is valid
        if (memory_address + DeviceId::SIZE <= 8192 && is_page_aligned(memory_address)) {
            eeprom.write(memory_address, reinterpret_cast<const uint8_t*>(&device_id), DeviceId::SIZE);
        }
    }

    //! @brief Get the DeviceId
    const DeviceId& get_device_id() const {
        return device_id;
    }

private:
    //! @brief Check if address is page-aligned (multiple of 32)
    bool is_page_aligned(uint16_t address) const {
        return (address % 32) == 0;
    }

    DeviceId device_id;      //!< Current DeviceId
    Eeprom& eeprom;          //!< Reference to EEPROM
    uint16_t memory_address; //!< Address for DeviceId storage
};

#endif // EEPROM_DEVICE_ID_H