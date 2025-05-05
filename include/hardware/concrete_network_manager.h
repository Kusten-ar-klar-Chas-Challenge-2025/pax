#ifndef CONCRETE_NETWORK_MANAGER_H
#define CONCRETE_NETWORK_MANAGER_H

#include "network_manager.h"
#include "physical_network.h"
#include "backend_network.h"
#include "physical_network_factory.h"

//! @brief Concrete network manager
//! Handles actual connections using Arduino's network interfaces
//! @note This is a singleton class
class ConcreteNetworkManager : public NetworkManager {
private:
    PhysicalNetwork* m_physical; // Pointer to manage dynamically created instance
    BackendNetwork& m_backend;
    ServerConfig m_serverConfig;
    NetworkSecrets m_networkSecrets;

public:
    //! @brief Constructor
    //! @param physical The physical network to use, use the call to the factory to get a new connection
    //! @param backend The backend network to use
    //! @param serverConfig The server configuration
    //! @param networkSecrets The network secrets
    ConcreteNetworkManager(PhysicalNetwork* physical, BackendNetwork& backend,
                           const ServerConfig& serverConfig, const NetworkSecrets& networkSecrets)
        : m_physical(physical), 
        m_backend(backend), 
        m_serverConfig(serverConfig), 
        m_networkSecrets(networkSecrets) 
    {}

    //! @brief Clean up the physical network
    ~ConcreteNetworkManager() override 
    {
        delete m_physical;
    }

    //! @brief Connect to the network
    void connect() override 
    {
        if (m_physical->connect(m_networkSecrets)) {
            m_backend.initialize(m_serverConfig);
        }
    }

    //! @brief Disconnect from the network
    void disconnect() override 
    {
        m_physical->disconnect();
    }

    //! @brief Send a message to the backend
    void send(const std::string_view& message) override 
    {
        if (m_physical->connected()) {
            m_backend.send(message);
        }
    }

    //! @brief Receive a message from the backend
    void receive(std::string& message) override 
    {
        if (m_physical->connected()) {
            m_backend.receive(message);
        }
    }
};

#endif