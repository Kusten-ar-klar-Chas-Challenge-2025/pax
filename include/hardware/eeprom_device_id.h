#ifndef EEPROM_DEVICE_ID_H
#define EEPROM_DEVICE_ID_H

#include "eeprom.h"
#include "uuid.h"
#include "device_id.h"

//! @brief Manages a DeviceId in EEPROM, reading on construction and writing on destruction if needed
class EepromDeviceId {
public:
    //! @brief Constructor: Read DeviceId from EEPROM or initialize with provided UUID
    //! @param uuid 16-byte UUID to use if EEPROM has no valid DeviceId
    //! @param eeprom Reference to EEPROM interface
    //! @param memory_address Page-aligned address to store/read DeviceId
    EepromDeviceId(const Uuid& uuid, Eeprom& eeprom, uint16_t memory_address)
        : m_eeprom(eeprom), m_memory_address(memory_address) {
        // Validate address
        if (m_memory_address + DeviceId::SIZE > 8192 || // Beyond EEPROM size
            !is_page_aligned(m_memory_address)) {       // Not page-aligned
            m_device_id = DeviceId(uuid); // Fall back to provided UUID
            return;
        }

        // Try to read existing DeviceId
        DeviceId read_id;
        EepromError result = m_eeprom.read(m_memory_address, reinterpret_cast<uint8_t*>(&read_id), DeviceId::SIZE);
        if (result == EepromError::SUCCESS && read_id.is_valid()) {
            m_device_id = read_id; // Use existing DeviceId
        } else {
            m_device_id = DeviceId(uuid); // Initialize with provided UUID
        }
    }

    //! @brief Destructor: Write DeviceId to EEPROM if no valid ID exists
    ~EepromDeviceId() {
        // Check if EEPROM has a valid DeviceId
        DeviceId read_id;
        EepromError result = m_eeprom.read(m_memory_address, reinterpret_cast<uint8_t*>(&read_id), DeviceId::SIZE);
        if (result == EepromError::SUCCESS && read_id.is_valid()) {
            return; // Valid ID exists, no need to write
        }

        // Write DeviceId if address is valid
        if (m_memory_address + DeviceId::SIZE <= 8192 && is_page_aligned(m_memory_address)) {
            m_eeprom.write(m_memory_address, reinterpret_cast<const uint8_t*>(&m_device_id), DeviceId::SIZE);
        }
    }

    //! @brief Get the DeviceId
    const DeviceId& get_device_id() const {
        return m_device_id;
    }

private:
    //! @brief Check if address is page-aligned (multiple of 32)
    bool is_page_aligned(uint16_t address) const {
        return (address % 32) == 0;
    }

    DeviceId m_device_id;      //!< Current DeviceId
    Eeprom& m_eeprom;          //!< Reference to EEPROM
    uint16_t m_memory_address; //!< Address for DeviceId storage
};

#endif // EEPROM_DEVICE_ID_H