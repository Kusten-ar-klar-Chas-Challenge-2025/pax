#ifndef CORE_INTERFACES_NETWORK_MANAGER_H
#define CORE_INTERFACES_NETWORK_MANAGER_H

#include <string>
#include <string_view>
#include <cstdint>
#include <cstddef>

//! @brief Server configuration for connecting to a server
struct ServerConfig
{
    //! @brief Host
    std::string_view host;
    //! @brief Port
    uint16_t port;
};

//! @brief Network secrets for connecting to a WiFi network
//! A concrete implementation of this struct will be provided by the hardware layer
//! @note this is just a string view into a hardware specific storage
struct NetworkSecrets
{
    //! @brief SSID
    std::string_view ssid;
    //! @brief Password
    std::string_view password;
};

//! @brief Network manager interface
//! @note This is a pure virtual class
class NetworkManager
{
    protected:
    //! @brief Protected constructor to prevent instantiation   
    NetworkManager() = default;
    public:
    //! @brief Delete copy constructor to prevent copying
    NetworkManager(const NetworkManager&) = delete;
    //! @brief Delete assignment operator to prevent assignment
    NetworkManager& operator=(const NetworkManager&) = delete;
    //! @brief Connect to the network
    virtual void connect() = 0;
    //! @brief Disconnect from the network
    virtual void disconnect() = 0;
    //! @brief Send a message to the network
    virtual void send(const std::string_view& message) = 0;
    //! @brief Receive a message from the network
    virtual void receive(std::string& message) = 0;
    //! @brief Destructor
    virtual ~NetworkManager() = default;
};

#endif

