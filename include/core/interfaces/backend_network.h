#ifndef CORE_INTERFACES_BACKEND_NETWORK_H
#define CORE_INTERFACES_BACKEND_NETWORK_H

#include "network_manager.h"

//! @brief Interface for backend communication (using a given network connection over some protocol)
class BackendNetwork {
protected:
    Client& m_network_connection;
    ServerConfig& m_config;
    BackendNetwork(Client& network_connection, ServerConfig& config) : m_network_connection(network_connection), m_config(config) {}
public:
    BackendNetwork(const BackendNetwork&) = delete;
    BackendNetwork& operator=(const BackendNetwork&) = delete;

    //! @brief Initialize the backend with server configuration
    virtual bool initialize(const ServerConfig& config) = 0;
    //! @brief Send a message to the backend
    virtual bool send(const std::string_view& message) = 0;
    //! @brief Receive a message from the backend
    virtual bool receive(std::string& message) = 0;

    virtual ~BackendNetwork() = default;
};

#endif