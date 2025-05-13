#ifndef ROOM_CONFIG_H
#define ROOM_CONFIG_H

#include <array>
#include "eeprom.h"
#include "eeprom_addresses.h"
#include <string_view>

namespace RoomConfig {
  static std::array<unsigned char, 32> room_id; //!< Room ID can be name or number, 32 bytes max. Retrieved from backend

  void store_room_id(const std::array<unsigned char, 32>& room_id, Eeprom& eeprom) {
    eeprom.write(EepromAddresses::RoomID, room_id.data(), sizeof(room_id));
  }

  void load_room_id(std::array<unsigned char, 32>& room_id, Eeprom& eeprom) {
    eeprom.read(EepromAddresses::RoomID, room_id.data(), sizeof(room_id));
  }
};


#endif