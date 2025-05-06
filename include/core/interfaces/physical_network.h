//! @file physical_network.h
//! @author Jennifer Gott
//! @date 2025-05-06
//! @brief Physical network interface
//! @note This is a pure virtual class, extend this class for your specific physical network implementation.

#ifndef CORE_INTERFACES_PHYSICAL_NETWORK_H
#define CORE_INTERFACES_PHYSICAL_NETWORK_H

#include "network_types.h"

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

    virtual ~PhysicalNetwork() = default;
};

#endif  // CORE_INTERFACES_PHYSICAL_NETWORK_H