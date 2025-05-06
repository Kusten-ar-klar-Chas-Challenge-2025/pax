//! @file ethernet_physical_network.h
//! @author Jennifer Gott
//! @date 2025-05-06
//! @brief Ethernet physical network interface
//! @todo Actual connection logic for different Ethernet shields

#ifndef ETHERNET_PHYSICAL_NETWORK_H
#define ETHERNET_PHYSICAL_NETWORK_H

#include <Ethernet.h> //!< Ethernet library
#include "physical_network.h" //!< Physical network interface

class EthernetPhysicalNetwork : public PhysicalNetwork {
 private:
  EthernetClient m_client;
  static inline bool is_ip_address_zero(const IPAddress& ip) { return ip == IPAddress(0,0,0,0); }

 public:
  EthernetPhysicalNetwork() = default;

  //! @brief Connect to the network
  //! @param secrets NetworkSecrets object containing SSID and password. Unused for Ethernet.
  //! @return NetworkError enum value
  [[nodiscard]] NetworkError connect([[maybe_unused]]const NetworkSecrets& secrets) override {
    //! Some example MAC address
    uint8_t mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
    Ethernet.begin(mac);
    
    if (Ethernet.linkStatus() == LinkON) 
    {
      if (is_ip_address_zero(Ethernet.localIP()))
      {
        //! If the IP address is 0.0.0.0, then we need to set the IP address manually
        //! This is a workaround for the Ethernet library bug
        //! https://github.com/arduino-libraries/Ethernet/issues/348
        Ethernet.begin(mac, IPAddress(192,168,1,100));
      }
      return NetworkError::OK;
    }

    return NetworkError::NOT_CONNECTED;
  }

  //! @brief Disconnect from the network
  //! @note This is a no-op for Ethernet
  void disconnect() override { }

  //! @brief Check if the network is connected
  //! @return true if the network is connected, false otherwise
  bool connected() const override 
  {
    return Ethernet.linkStatus() == LinkON && !is_ip_address_zero(Ethernet.localIP());
  }

  Client& get_network_connection() { return m_client; }
};

#endif  // ETHERNET_PHYSICAL_NETWORK_H