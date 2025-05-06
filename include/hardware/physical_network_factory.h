//! @file physical_network_factory.h
//! @author Jennifer Gott
//! @date 2025-05-06
//! @brief Physical network factory
//! This is a factory class for creating physical network interfaces.
//! Call the static method create_network to get a pair of physical and backend networks.
//! @note Only do this once and only in the Arduino context, not in unit tests.

#ifndef PHYSICAL_NETWORK_FACTORY_H_
#define PHYSICAL_NETWORK_FACTORY_H_

#include <utility>
#include "backend_network.h"
#include "ethernet_physical_network.h"
#include "http_backend_network.h"
#include "wifi_physical_network.h"

//! @brief Physical network factory
//! Pass the value of the configuration pin to determine which network to use.
//! Only use this in the Arduino context, not in unit tests.
class PhysicalNetworkFactory {
private:
//! @brief Don't instantiate this class, use the static method create_network
 PhysicalNetworkFactory() = default;
 public:
  //! @brief Create a network pair
  //! @param use_wifi Whether to use WiFi
  //! @param config Server configuration
  //! @return A pair of physical (PhysicalNetwork&) and backend (BackendNetwork&) networks
  //! @warning Do not store the returned pair, use it immediately
  //! Otherwise, you will get a dangling reference
  
  [[nodiscard]] static std::pair<PhysicalNetwork&, BackendNetwork&> create_network(bool use_wifi,
                                                                                   const ServerConfig& config) {
    static WiFiPhysicalNetwork wifi_physical;
    static EthernetPhysicalNetwork ethernet_physical;
    if (use_wifi) {
      static HttpBackendNetwork wifi_backend(wifi_physical.get_network_connection(), config);
      return std::pair<PhysicalNetwork&, BackendNetwork&>(wifi_physical, wifi_backend);
    } 
    else 
    {
      static HttpBackendNetwork ethernet_backend(ethernet_physical.get_network_connection(), config);
      return std::pair<PhysicalNetwork&, BackendNetwork&>(ethernet_physical, ethernet_backend);
    }
  }
};

#endif  // PHYSICAL_NETWORK_FACTORY_H_