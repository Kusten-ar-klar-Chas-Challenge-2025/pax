#ifndef MOCK_PHYSICAL_NETWORK_H
#define MOCK_PHYSICAL_NETWORK_H

#ifdef UNIT_TEST

#include "physical_network.h"
#include <string_view>

class MockPhysicalNetwork final : public PhysicalNetwork {
    private:
        bool m_connected{false};
        bool m_next_connect_result{true};
        std::string_view m_valid_ssid{"test_network"};
        std::string_view m_valid_password{"correct_password"};

    public:
        MockPhysicalNetwork() = default;

        void set_next_connect_result(bool result) {
            m_next_connect_result = result;
        }

        void set_valid_credentials(std::string_view ssid, std::string_view password) {
            m_valid_ssid = ssid;
            m_valid_password = password;
        }

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

        void disconnect() override {
            m_connected = false;
        }

        bool connected() const override {
            return m_connected;
        }
};

#endif  // UNIT_TEST

#endif  // MOCK_PHYSICAL_NETWORK_H