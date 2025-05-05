#ifndef HTTP_BACKEND_NETWORK_H
#define HTTP_BACKEND_NETWORK_H


#include <ArduinoHttpClient.h>  
#include "backend_network.h"

class HttpBackendNetwork : public BackendNetwork {
private:
    HttpClient m_httpClient;
    std::string_view m_endpoint;   // Store API path

public:
    HttpBackendNetwork(Client& network_connection, ServerConfig& config) : 
        BackendNetwork(network_connection, config),
        m_httpClient(m_network_connection, m_config.host.data(), m_config.port)
    {
    }

    bool initialize(const ServerConfig& config) override {
        m_endpoint = "/api";
        return true;
    }

    bool send(const std::string_view& message) override {
        m_httpClient.beginRequest();
        m_httpClient.post(m_endpoint.data(), "application/json", message.data());
        m_httpClient.sendHeader("Host", m_config.host.data());
        int httpCode = m_httpClient.responseStatusCode();
        m_httpClient.endRequest();
        return httpCode == 200;  // Success for HTTP OK
    }

    bool receive(std::string& message) override {
        m_httpClient.beginRequest();
        m_httpClient.get(m_endpoint.data());
        m_httpClient.sendHeader("Host", m_config.host.data());
        int httpCode = m_httpClient.responseStatusCode();
        if (httpCode == 200) {
            message = m_httpClient.responseBody().c_str();
            m_httpClient.endRequest();
            return true;
        }
        m_httpClient.endRequest();
        return false;
    }
};

#endif