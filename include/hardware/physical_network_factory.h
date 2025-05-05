#ifndef PHYSICAL_NETWORK_FACTORY_H
#define PHYSICAL_NETWORK_FACTORY_H

#include "wifi_physical_network.h"
#include "ethernet_physical_network.h"
#include "pin_config.h"

//! @brief Physical network factory
//! @note Only used for static functions and cannot be instantiated
class PhysicalNetworkFactory {
private:
    //! @brief Private constructor to prevent instantiation
    PhysicalNetworkFactory() = default;
public:
    //! @brief Only call this if we need to create a new connection
    //! 
    static PhysicalNetwork* new_connection() {
        bool useWiFi = digitalRead(PinConfig::pin_network_select) == HIGH; // HIGH for WiFi, LOW for Ethernet

        if (useWiFi) {
            return new WiFiPhysicalNetwork();
        } else {
            return new EthernetPhysicalNetwork();
        }
    }
};

#endif