#ifndef EEPROM_ADDRESSES_H
#define EEPROM_ADDRESSES_H

#include <cstdint>


//! @brief Addresses for the EEPROM based on offsets between storage locations
//! @note The EEPROM is 8192 bytes, and is page-aligned at 32 byte intervals
//! @note Maximum address is 0x2000
namespace EepromAddresses {

  static constexpr uint16_t PageSize = 32; //!< Page size in bytes

  //! Device ID is 28 bytes, aligned to 32 bytes for 32-bit write cache
  static constexpr uint16_t DeviceID = 0x00;
  //! We offset by 32 bytes to align to 32-bit write cache
  static constexpr uint16_t RoomID = DeviceID + 32;
  //! Next available address after Room ID, also aligned, since room id is 32 bytes
  static constexpr uint16_t NextAvailable = RoomID + 32;
  static constexpr uint16_t End = 0x2000; //!< End of EEPROM. Do not write here, the last actual address is -1 of this. Think of it like iterator end()

  //! @brief Check if an address is within the valid range
  //! @param address The address to check
  //! @return True if the address is within the valid range, false otherwise
  static constexpr bool is_valid(uint16_t address) {
    return address >= DeviceID && address < End;
  }

  //! @brief Check if an address is page-aligned
  //! @param address The address to check
  //! @return True if the address is page-aligned, false otherwise
  static constexpr bool is_page_aligned(uint16_t address) {
    return (address % PageSize) == 0;
  }
}

#endif