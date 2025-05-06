//! @file main.cpp
//! @author Jennifer Gott
//! @date 2025-05-06
//! @brief Pax Occupancy sensor
//! @details This program is used to read the occupancy of a room and send the data to the backend
//! For the Arduino R4 Wifi.


#include <Arduino.h> //!< Arduino library
#include <string_view> //!< String view
#include "backend_network.h" //!< Backend network interface
#include "network_types.h" //!< Network types
#include "physical_network_factory.h" //!< Factory for creating physical networks
#include "arduino_pin_io.h" //!< Arduino pin IO implementation, also includes the pin config
#include "arduino_secrets.h" //!< Arduino secrets constants for SSID and PASSWORD and SERVER_HOST and SERVER_PORT

ArduinoPinIO pin_io;

void setup() 
{
  pin_io.setup();
  Serial.begin(115200);

  NetworkSecrets secrets = {SSID, PASSWORD};
  ServerConfig config = {SERVER_HOST, SERVER_PORT};

  //! Read the network select pin to determine if the network is wifi or ethernet
  bool use_wifi = pin_io.digitalRead(PinConfig::pin_network_select) == HIGH;


  //! Structured binding of a std::pair to get the physical and backend network objects from the factory
  auto [physical, backend] = PhysicalNetworkFactory::create_network(use_wifi, config);

  physical.connect(secrets);

  if (physical.connected()) 
  {
    char config_response[256];
    auto result = backend.request(HttpMethod::GET, "api/config", "", config_response,
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

    char time_response[256];
    result = backend.request(HttpMethod::GET, "api/time", "", time_response,
                             sizeof(time_response));
    if (!is_error(result)) 
    {
      Serial.print(F("Time: "));
      Serial.println(time_response);
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