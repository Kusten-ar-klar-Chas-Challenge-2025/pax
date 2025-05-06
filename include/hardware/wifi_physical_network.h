//! @file wifi_physical_network.h
//! @author Jennifer Gott
//! @date 2025-05-06
//! @brief WiFi physical network implementation
//! @note This is a singleton class
//! Wraps a WiFiClient object and provides a physical network interface.
//! Handles connectivity and status checking.

#ifndef WIFI_PHYSICAL_NETWORK_H
#define WIFI_PHYSICAL_NETWORK_H

#include <WiFiS3.h>
#include "physical_network.h"

//! @brief WiFi physical network implementation
//! @note This is a singleton class
//! Wraps a WiFiClient object and provides a physical network interface
//! Handles connectivity and status checking
class WiFiPhysicalNetwork : public PhysicalNetwork 
{
    private:
        WiFiClient network_connection;
        static constexpr int MAX_ATTEMPTS = 10; //!< Maximum number of attempts to connect to the network
    public:
    //! @brief Connect to the network
    //! @param secrets NetworkSecrets object containing SSID and password
    //! @return NetworkError enum value
    //! @note This can take a while to connect! Each attempt waits 1 second.
    NetworkError connect(const NetworkSecrets& secrets) override 
    {
        WiFi.begin(secrets.ssid.data(), secrets.password.data());
        int attempts = 0;
        while (WiFi.status() != WL_CONNECTED && attempts < MAX_ATTEMPTS) {
            delay(1000);
            attempts++;
        }
        switch (WiFi.status()) {
            case WL_CONNECTED:
                return NetworkError::OK;
            case WL_CONNECT_FAILED:
                return NetworkError::CONNECT_FAILED;
            default:
                return NetworkError::UNKNOWN;
        }
    }
    //! @brief Disconnect from the network
    void disconnect() override 
    {
        WiFi.disconnect();
    }
    //! @brief Check if the network is connected    
    bool connected() const override 
    {
        return WiFi.status() == WL_CONNECTED;
    }
    //! @brief Get the underlying Client object
    //! @return Reference to the WiFiClient object
    //! @note Use by taking a Client& from the PhysicalNetwork interface
    Client& get_network_connection() const {
        return network_connection;
    }
};

#endif