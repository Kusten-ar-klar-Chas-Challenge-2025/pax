#ifndef MOCK_BACKEND_NETWORK_H
#define MOCK_BACKEND_NETWORK_H


#include <string>
#include <cstring>
#include "backend_network.h"
#include <vector>
class MockBackendNetwork : public BackendNetwork
{
    public:
    struct Request {
        HttpMethod method;
        std::string url;
        const char* body;
        std::string response;
        size_t response_size;
    };

    std::vector<Request> requests;

    explicit MockBackendNetwork(const ServerConfig& config) : m_config(config) {}

    NetworkError request(HttpMethod method,
               const std::string_view& endpoint,
               const std::string_view& request_body,
               char* response_buffer,
               size_t max_len) override 
    {
        if (method == HttpMethod::GET && (!response_buffer || max_len == 0)) 
        {
            return NetworkError::NO_BUFFER_PROVIDED;
        }
        if (method != HttpMethod::GET && request_body.empty()) 
        {
            return NetworkError::NO_REQUEST_MESSAGE;
        }

        Request req;
        req.method = method;
        req.url = endpoint;
        req.body = request_body.data();
        req.response_size = max_len;
        if (!requests.empty() && requests.back().response.size() <= max_len) {
            req.response = requests.back().response;
            std::strncpy(response_buffer, req.response.c_str(), max_len);
        }
        requests.push_back(req);
    }

        // Set the next response for testing
    void set_response(const std::string& response) {
        requests.push_back({HttpMethod::GET, "", nullptr, response, 0});
    }

    private:
    const ServerConfig& m_config;
    
};

#endif  // MOCK_BACKEND_NETWORK_H