#ifndef WIFI_PHYSICAL_NETWORK_H
#define WIFI_PHYSICAL_NETWORK_H

#include <WiFiS3.h>
#include "physical_network.h"

//! @brief WiFi physical network implementation
class WiFiPhysicalNetwork : public PhysicalNetwork {
public:
    //! @brief Connect to the network
    bool connect(const NetworkSecrets& secrets) override {
        WiFi.begin(secrets.ssid.data(), secrets.password.data());
        int attempts = 0;
        while (WiFi.status() != WL_CONNECTED && attempts < 10) {
            delay(1000);
            attempts++;
        }
        return WiFi.status() == WL_CONNECTED;
    }
    //! @brief Disconnect from the network
    void disconnect() override {
        WiFi.disconnect();
    }
    //! @brief Check if the network is connected    
    bool connected() const override {
        return WiFi.status() == WL_CONNECTED;
    }
};

#endif