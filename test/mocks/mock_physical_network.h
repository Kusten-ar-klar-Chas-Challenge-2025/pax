//! @file mock_physical_network.h
//! @brief Mock implementation of the PhysicalNetwork interface
//! @author Jennifer Gott (simbachu@gmail.com)
//! @date 2025-05-06

#ifndef MOCK_PHYSICAL_NETWORK_H
#define MOCK_PHYSICAL_NETWORK_H

#include "physical_network.h"
#include <string_view>

//! @brief Mock implementation of the PhysicalNetwork interface
//! The real implementation wraps aClient object of the appropriate type
//! for the underlying network technology.
//! This mock implementation is for testing purposes only.
class MockPhysicalNetwork final : public PhysicalNetwork {
    private:
        bool m_connected{false};
        bool m_next_connect_result{true};
        std::string_view m_valid_ssid{"test_network"};
        std::string_view m_valid_password{"correct_password"};

    public:
        //! @brief Default constructor
        MockPhysicalNetwork( ) = default;


        //! @brief Constructor with valid credentials and optional connection status
        //! @param secrets NetworkSecrets object containing SSID and password
        //! @param already_connected Optional boolean to set initial connection status
        MockPhysicalNetwork( NetworkSecrets secrets, bool already_connected = false ) 
            : m_valid_ssid(secrets.ssid), m_valid_password(secrets.password), m_connected(already_connected) 
            {

            }


        //! @brief Set the next connect result
        //! @param result Boolean to set the next connect result
        void set_next_connect_result(bool result) {
            m_next_connect_result = result;
        }


        //! @brief Set the valid credentials
        //! @param ssid String view of the SSID
        //! @param password String view of the password
        void set_valid_credentials(std::string_view ssid, std::string_view password) {
            m_valid_ssid = ssid;
            m_valid_password = password;
        }


        //! @brief Connect to the network
        //! @param secrets NetworkSecrets object containing SSID and password
        //! @return NetworkError enum value
        [[nodiscard]] NetworkError connect(const NetworkSecrets& secrets) override {
            bool credentials_valid = (secrets.ssid == m_valid_ssid && 
                                   secrets.password == m_valid_password);
            if (!credentials_valid)
            {
                return NetworkError::INVALID_CREDENTIALS;
            }
            m_connected = m_next_connect_result && credentials_valid;
            return m_connected ? NetworkError::OK : NetworkError::NOT_CONNECTED;
        }


        //! @brief Disconnect from the network
        void disconnect() override {
            m_connected = false;
        }


        //! @brief Check if the network is connected
        //! @return Boolean value indicating if the network is connected
        bool connected() const override {
            return m_connected;
        }
};


#endif  // MOCK_PHYSICAL_NETWORK_H