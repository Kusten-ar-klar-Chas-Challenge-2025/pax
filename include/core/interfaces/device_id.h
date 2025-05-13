#ifndef INTERFACES_DEVICE_ID_H
#define INTERFACES_DEVICE_ID_H

#include <cstdint>
#include <cstring>
#include <array>
#include "uuid.h"

//! @brief Device ID format for storage in 24LC64 EEPROM (28 bytes)
struct DeviceId {
    char prefix[4];        //!< ASCII prefix "DID:" (4 bytes)
    uint8_t version;       //!< Format version (1 byte)
    uint8_t unique_id[16]; //!< 128-bit UUID (16 bytes)
    uint8_t reserved[5];   //!< Reserved for future use (5 bytes)
    uint16_t checksum;     //!< CRC16 checksum (2 bytes)

    static constexpr size_t SIZE = 28; //!< Total size in bytes

    DeviceId() : prefix(), version(0x01), unique_id(), reserved(), checksum() {
        std::memcpy(prefix, "DID:", 4);
        std::memset(unique_id, 0, 16);
        std::memset(reserved, 0, 5);
        checksum = compute_crc16();
    }

    DeviceId(const DeviceId& other) : prefix(), version(other.version), unique_id(), reserved(), checksum() {
        std::memcpy(prefix, other.prefix, 4);
        std::memcpy(unique_id, other.unique_id, 16);
        std::memcpy(reserved, other.reserved, 5);
        checksum = other.checksum;
    }

    //! @brief Initialize the device ID with a UUID
    DeviceId(const Uuid& uuid) : prefix(), version(0x01), unique_id(), reserved(), checksum() {
        std::memcpy(prefix, "DID:", 4);
        std::memcpy(unique_id, uuid.bytes.data(), 16);
        std::memset(reserved, 0, 5);
        checksum = compute_crc16();
    }

    //! @brief Compute CRC16-CCITT checksum of the device ID
    uint16_t compute_crc16() const {
        // Simplified CRC16-CCITT implementation (polynomial 0x1021)
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

#endif // INTERFACES_DEVICE_ID_H