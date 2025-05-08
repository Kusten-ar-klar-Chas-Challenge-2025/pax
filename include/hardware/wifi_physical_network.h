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
        WiFiClient m_client;
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
                return NetworkError::CONNECTION_TIMEOUT;
            default:
                return NetworkError::UNKNOWN;
        }
    }
    //! @brief Disconnect from the network
    void disconnect() override 
    {
        m_client.stop();
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
    Client& get_network_connection()  {
        return m_client;
    }
    //! @brief Get the MAC address of the network interface
    //! @param[out] mac_address Reference to a fixed size array to store the MAC address
    //! @return True if the MAC address was successfully retrieved, false otherwise
    bool get_mac_address(std::array<uint8_t, 6>& mac_address) const override {
        if (WiFi.status() != WL_NO_MODULE)
        {
            return false;
        }
        WiFi.macAddress(mac_address.data());
        return true;
    }
};

#endif