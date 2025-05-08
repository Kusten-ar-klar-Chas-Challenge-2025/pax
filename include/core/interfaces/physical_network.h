//! @file physical_network.h
//! @author Jennifer Gott
//! @date 2025-05-06
//! @brief Physical network interface
//! @note This is a pure virtual class, extend this class for your specific physical network implementation.

#ifndef CORE_INTERFACES_PHYSICAL_NETWORK_H
#define CORE_INTERFACES_PHYSICAL_NETWORK_H

#include "network_types.h"
#include <array> // for get_mac_address
//! @brief Physical network interface
//! @note This is a pure virtual class
class PhysicalNetwork {
protected:
    //! @brief Protected constructor to prevent instantiation
    PhysicalNetwork() = default;
public:
    //! @brief Delete copy constructor to prevent copying
    PhysicalNetwork(const PhysicalNetwork&) = delete;
    //! @brief Delete assignment operator to prevent assignment
    PhysicalNetwork& operator=(const PhysicalNetwork&) = delete;

    //! @brief Connect to the network
    //! @param secrets The network secrets
    //! @return The error code
    //! @usage auto error = network.connect(secrets);
    //! if (is_error(error)) {
    //!     // handle the error
    //! }
    virtual NetworkError connect(const NetworkSecrets& secrets) = 0;
    //! @brief Disconnect from the network
    virtual void disconnect() = 0;
    //! @brief Check if the network is connected
    virtual bool connected() const = 0;

    //! @brief Get the MAC address of the network interface
    //! @param[out] mac_address Pointer to a buffer to store the MAC address
    //! @return True if the MAC address was successfully retrieved, false otherwise
    virtual bool get_mac_address(std::array<uint8_t, 6>& mac_address) const = 0;

    virtual ~PhysicalNetwork() = default;
};

#endif  // CORE_INTERFACES_PHYSICAL_NETWORK_H