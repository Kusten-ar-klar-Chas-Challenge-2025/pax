//! @file main.cpp
//! @author Jennifer Gott
//! @date 2025-05-06
//! @brief Pax Occupancy sensor
//! @details This program is used to read the occupancy of a room and send the data to the backend
//! For the Arduino R4 Wifi.


#include <Arduino.h> //!< Arduino library
#include <string_view> //!< String view
#include "eeprom_device_id.h"
#include "eeprom_addresses.h"
#include "room_config.h"
#include "uuid.h"
#include "arduino_random_source.h"
#include "mcp7940n_clock.h"
#include "backend_network.h" //!< Backend network interface
#include "network_types.h" //!< Network types
#include "physical_network_factory.h" //!< Factory for creating physical networks
#include "arduino_pin_io.h" //!< Arduino pin IO implementation, also includes the pin config
#include "arduino_secrets.h" //!< Arduino secrets constants for SSID and PASSWORD and SERVER_HOST and SERVER_PORT

ArduinoPinIO pin_io;
Mcp7940nClock clock_source;
ArduinoRandomSource random_source;

void setup() 
{
  pin_io.setup();
  Serial.begin(115200);

  clock_source.begin();

  NetworkSecrets secrets = {SSID, PASSWORD};
  ServerConfig config = {SERVER_HOST, SERVER_PORT};

  //! Read the network select pin to determine if the network is wifi or ethernet
  bool use_wifi = pin_io.digitalRead(PinConfig::Pin::NETWORK_SELECT) == HIGH;


  //! Structured binding of a std::pair to get the physical and backend network objects from the factory
  auto [physical, backend] = PhysicalNetworkFactory::create_network(use_wifi, config);

  std::array<uint8_t, 6> mac_address;
  physical.get_mac_address(mac_address);

  DeviceId device_id (Uuid(mac_address, random_source, clock_source, pin_io));

  physical.connect(secrets);


  if (physical.connected()) 
  {
    Uuid uuid;
    if (device_id.is_valid()){
      uuid = device_id; //! copy the UUID bits from the device id
    }
    else {
      uuid = Uuid(); //! blank uuid
    }

    //! Build the config http path using the UUID
    char config_http_path[256] = "api/config/";
    char* current = config_http_path + strlen(config_http_path);
    for (const auto& byte : uuid.bytes) {
        current += snprintf(current, sizeof(config_http_path) - (current - config_http_path), "%02X", byte);
    }
    *current = '\0';

    char config_response[256];

    //! Get the config from the backend
    auto result = backend.request(HttpMethod::GET, std::string_view(config_http_path), "", config_response,
                                  sizeof(config_response));
    if (!is_error(result)) 
    {
      Serial.print(F("Config: "));
      Serial.println(config_response);
    } 
    else 
    {
      Serial.print(F("Config request failed: "));
      Serial.println(static_cast<int>(result));
    }

    //! Some json handling to get the config data: Either blank /not configured on backend yet, keep trying to get config every 15 minutes
    //! if config is not blank, update the config

    //! Get the time from the backend
    char time_response[256];
    result = backend.request(HttpMethod::GET, "api/time", "", time_response,
                             sizeof(time_response));

  
    if (!is_error(result)) 
    {
      Serial.print(F("Time: "));
      Serial.println(time_response);

      //! read the time string as YYYY-MM-DD HH:MM:SS
      std::string_view time_string(time_response);
      uint8_t year = atoi(time_string.substr(0, 4).data());
      uint8_t month = atoi(time_string.substr(5, 2).data());
      uint8_t day = atoi(time_string.substr(8, 2).data());
      uint8_t hour = atoi(time_string.substr(11, 2).data());
      uint8_t minute = atoi(time_string.substr(14, 2).data());
      uint8_t second = atoi(time_string.substr(17, 2).data());

      DateTime time(year, month, day, hour, minute, second);
      clock_source.set_time(time);
    } 
    else 
    {
      Serial.print(F("Time request failed: "));
      Serial.println(static_cast<int>(result));
    }


    char room_id[16];
    snprintf(room_id, sizeof(room_id), "api/rooms/%d", 101);
    std::string_view message = "{\"occupied\":true}";
    result = backend.request(HttpMethod::PUT, room_id, message, nullptr, 0);
    if (!is_error(result)) 
    {
      Serial.println(F("Room status updated"));
    } else {
      Serial.print(F("Room update failed: "));
      Serial.println(static_cast<int>(result));
    }
  } else 
  {
    Serial.println(F("Network connection failed"));
  }
}

void loop() {}