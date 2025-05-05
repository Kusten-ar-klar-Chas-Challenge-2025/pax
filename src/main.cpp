#include <Arduino.h>

#include "arduino_secrets.h"
#include "arduino_sensor_manager.h"
#include "arduino_pin_io.h"
#include "concrete_network_manager.h"

ArduinoPinIO pin_io;
ArduinoSensorManager sensor_manager(&pin_io);
uint32_t last_reading_time = 0;

ServerConfig server_config{
  std::string_view(SERVER_HOST), 
  SERVER_PORT
};

NetworkSecrets network_secrets{
  std::string_view(SSID),
  std::string_view(PASSWORD)
};

HttpBackendNetwork http_backend_network;
ConcreteNetworkManager network_manager(
  PhysicalNetworkFactory::new_connection(), 
  http_backend_network, 
  server_config,
  network_secrets
);

void setup() {
  Serial.begin(115200);
  pin_io.setup();
  sensor_manager.request_reading();
  network_manager.connect();
}

void loop() {
  SensorReading sensor_reading = sensor_manager.read();
  if ( sensor_reading.time_ms > last_reading_time )
  {
    last_reading_time = sensor_reading.time_ms;
    Serial.println(sensor_reading.temperature);
    network_manager.send(std::to_string(sensor_reading.temperature));
  } 
}