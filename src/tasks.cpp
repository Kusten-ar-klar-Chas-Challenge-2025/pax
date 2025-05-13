#include <array>
#include <ArduinoJson.h>
#include <algorithm>
#include "tasks.h"
#include "http_backend_network.h"
#include "physical_network.h"
#include "eeprom_device_id.h"
#include "uuid.h"

namespace Tasks 
{
    namespace Device 
    {
        //! @brief Retrieve the device ID from the EEPROM
        //! @param uuid[out] The UUID to store the device ID in
        //! @param eeprom[in] The EEPROM interface
        void retrieve_uuid(Uuid& uuid, Eeprom& eeprom)
        {
            //! Get the device ID from the EEPROM
            EepromDeviceId device_id(uuid, eeprom, EepromAddresses::DeviceID);
            if (device_id.get_device_id().is_valid())
            {
                uuid = device_id.get_device_id();
            }
            else
            {
                //! Generate a blank
                uuid = Uuid();
            }
        }
    } // namespace Device

    namespace Config {
        enum class ConfigState {
            Valid,
            BackendNotSetup,
            NoInternetConnection,
            InvalidJson,
            InvalidRoomId,
            EepromWriteFailed
        };

        ConfigState retrieve_config(Uuid& uuid, Eeprom& eeprom, PhysicalNetwork& physical, HttpBackendNetwork& backend) {
            // Read room ID from EEPROM
            std::array<char, 32> room_id = {0}; // Initialize to zero
            eeprom.read(EepromAddresses::RoomID, reinterpret_cast<uint8_t*>(room_id.data()), room_id.size());

            if (!physical.connected()) {
                return ConfigState::NoInternetConnection;
            }

            // Check if room_id is all zeros
            bool is_empty = std::all_of(room_id.begin(), room_id.end(), [](char c) { return c == 0; });

            if (is_empty) {
                // Fetch room ID from backend using UUID
                JsonDocument json; // Adjust size as needed
                std::array<char, 64> buffer;
                std::array<char, 1024> response = {0};
                snprintf(buffer.data(), buffer.size(), "/api/config/%s", uuid.to_string().data());
                backend.request(HttpMethod::GET, buffer.data(), nullptr, response.data(), response.size());

                DeserializationError error = deserializeJson(json, response.data());
                if (error) {
                    return ConfigState::InvalidJson;
                }

                if (json.isNull()) {
                    return ConfigState::BackendNotSetup;
                }

                if (!json["room_id"].is<const char*>()) {
                    return ConfigState::InvalidRoomId;
                }

                const char* room_id_str = json["room_id"] | "";
                if (strlen(room_id_str) == 0) {
                    return ConfigState::BackendNotSetup;
                }

                // Copy room_id, ensuring null termination
                strncpy(room_id.data(), room_id_str, room_id.size() - 1);
                room_id.back() = '\0';

                // Save to EEPROM
                if (eeprom.write(EepromAddresses::RoomID, reinterpret_cast<uint8_t*>(room_id.data()), room_id.size()) != EepromError::SUCCESS) {
                    return ConfigState::EepromWriteFailed;
                }
            }

            // Validate room ID with backend
            StaticJsonDocument<256> json;
            std::array<char, 64> buffer;
            std::array<char, 1024> response = {0};
            snprintf(buffer.data(), buffer.size(), "/api/room/%s", room_id.data());
            backend.request(HttpMethod::GET, buffer.data(), nullptr, response.data(), response.size());

            DeserializationError error = deserializeJson(json, response.data());
            if (error) {
                return ConfigState::InvalidJson;
            }

            if (json.isNull() || !json["room_id"].is<const char*>()) {
                return ConfigState::BackendNotSetup;
            }

            const char* backend_room_id = json["room_id"] | "";
            if (strlen(backend_room_id) == 0) {
                return ConfigState::BackendNotSetup;
            }

            // Compare room IDs
            if (strncmp(backend_room_id, room_id.data(), room_id.size()) == 0) {
                return ConfigState::Valid;
            }

            return ConfigState::InvalidRoomId; // Room ID mismatch
        }
    } // namespace Config
};