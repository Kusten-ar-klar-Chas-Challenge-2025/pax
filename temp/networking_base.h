#ifndef NETWORKING_BASE_H
#define NETWORKING_BASE_H

#include "arduino_secrets.h"
#include <Arduino.h>
#include <WiFi.h>
#include <Ethernet.h>
#include "Device.h"

constexpr uint8_t NETWORK_CONFIG_PIN = 7;

class NetworkingBase {
private:
    WiFiClient* wifi_ptr = nullptr;
    EthernetClient* ethernet_ptr = nullptr;
    bool wifi_pin_set;
    bool configured = false;
public:
    NetworkingBase();
    NetworkingBase( WiFiClient* wifi_in, EthernetClient* ethernet_in );
    Stream * out_stream() const;
    Client * current_client() const;
    const bool wifi_on ( ) const;
    const bool ready_for_traffic ( ) const; 
    void begin();
    void operator ()();
    // ~NetworkingBase();
};

#endif