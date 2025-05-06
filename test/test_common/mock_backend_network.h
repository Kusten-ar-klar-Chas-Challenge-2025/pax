#ifndef MOCK_BACKEND_NETWORK_H
#define MOCK_BACKEND_NETWORK_H

#ifdef UNIT_TEST

#include <string>
#include <cstring>
#include "backend_network.h"

class MockBackendNetwork : public BackendNetwork 
{
    private:
    const ServerConfig& m_config;
    std::string m_last_message;
    std::string m_last_endpoint;
    HttpMethod m_last_method;

    public:
    explicit MockBackendNetwork(const ServerConfig& config) : m_config(config) {}

    bool request(HttpMethod method,
               const std::string_view& endpoint,
               const std::string_view& request_message,
               char* response_buffer,
               size_t max_len) override 
    {
        if (method == HttpMethod::GET && (!response_buffer || max_len == 0)) 
        {
            return false;
        }
        if (method != HttpMethod::GET && request_message.empty()) 
        {
            return false;
        }

        m_last_method = method;
        m_last_endpoint = endpoint;
        m_last_message = request_message;

        if (method == HttpMethod::GET && response_buffer) 
        {
            std::string response = "Mock response for " + m_last_endpoint;
            size_t bytes_to_copy = std::min(response.length(), max_len - 1);
            std::memcpy(response_buffer, response.data(), bytes_to_copy);
            response_buffer[bytes_to_copy] = '\0';
            return bytes_to_copy > 0;
        }
        return true;
    }

    std::string get_last_message() const { return m_last_message; }
    std::string get_last_endpoint() const { return m_last_endpoint; }
    HttpMethod get_last_method() const { return m_last_method; }
};

#endif  // UNIT_TEST

#endif  // MOCK_BACKEND_NETWORK_H