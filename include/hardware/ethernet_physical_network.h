#ifndef ETHERNET_PHYSICAL_NETWORK_H
#define ETHERNET_PHYSICAL_NETWORK_H

#include <Ethernet.h>
#include "physical_network.h"

class EthernetPhysicalNetwork : public PhysicalNetwork {
private:
    //! @brief MAC address for the Ethernet shield
    //! @note This is a static MAC address for the Ethernet shield
    //! @note This is a placeholder and should be replaced with the actual MAC address
    uint8_t mac[6] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

public:
    //! @brief Connect to the network
    bool connect([[maybe_unused]] const NetworkSecrets& secrets) override {
        //! @note Ethernet typically doesn't use SSID/password
        if (Ethernet.begin(mac) == 0) {
            return false; // Failed to configure Ethernet using DHCP
        }
        return true;
    }
    //! @brief Disconnect from the network
    void disconnect() override {
        //! @note Ethernet doesn't need to be explicitly disconnected
    }
    //! @brief Check if the network is connected
    bool connected() const override {
        return Ethernet.linkStatus() == LinkON;
    }
};

#endif