#ifndef CORE_INTERFACES_PHYSICAL_NETWORK_H
#define CORE_INTERFACES_PHYSICAL_NETWORK_H

#include "network_manager.h"

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
    virtual bool connect(const NetworkSecrets& secrets) = 0;
    //! @brief Disconnect from the network
    virtual void disconnect() = 0;
    //! @brief Check if the network is connected
    virtual bool connected() const = 0;

    virtual ~PhysicalNetwork() = default;
};

#endif