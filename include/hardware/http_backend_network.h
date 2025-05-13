//! @file http_backend_network.h
//! @author Jennifer Gott
//! @date 2025-05-06
//! @brief HTTP backend network implementation
//! @note This is a singleton class
//! Wraps a HttpClient object and provides a backend network interface.

#ifndef HTTP_BACKEND_NETWORK_H
#define HTTP_BACKEND_NETWORK_H

#include <ArduinoHttpClient.h> //!< HTTP client library
#include "backend_network.h" //!< Backend network interface

class HttpBackendNetwork : public BackendNetwork {
 private:
  HttpClient m_http_client;
  const ServerConfig& m_config;

 public:
  HttpBackendNetwork(Client& client, const ServerConfig& config)
      : m_http_client(client, config.host.data(), config.port),
        m_config(config) {}

  [[nodiscard]] NetworkError request(HttpMethod method,
               const std::string_view& endpoint,
               const std::string_view& request_message,
               char* response_buffer,
               size_t max_len) override 
  {
    if (method == HttpMethod::GET && (!response_buffer || max_len == 0)) 
    {
      return NetworkError::NO_BUFFER_PROVIDED;
    }
    if (method != HttpMethod::GET && request_message.empty()) 
    {
      return NetworkError::NO_REQUEST_MESSAGE;
    }

    //! Connect to the server using the ServerConfig
    if (!m_http_client.connect(m_config.host.data(), m_config.port))
    {
      return NetworkError::NOT_CONNECTED;
    }

    m_http_client.beginRequest();
    
    switch(method) 
    {
      case HttpMethod::GET:
        m_http_client.get(endpoint.data());
        break;

      case HttpMethod::PUT:
        m_http_client.put(endpoint.data(), "application/json", request_message.data());
        break;

      case HttpMethod::POST:
        m_http_client.post(endpoint.data(), "application/json", request_message.data());
        break;

      default:
        return NetworkError::HTTP_ERROR;
    }
    
    m_http_client.sendHeader("Host", m_config.host.data());
    
    int http_code = m_http_client.responseStatusCode();

    bool success = (http_code == 200);
    
    //! If the request is a GET and a buffer is provided, read the response
    if (success && method == HttpMethod::GET && response_buffer) 
    {
      size_t bytes_read = 0;
      //! Read the response until the buffer is full
      while (m_http_client.available() && bytes_read < max_len - 1) 
      {
        response_buffer[bytes_read++] = m_http_client.read();
      }
      //! Null terminate the response
      response_buffer[bytes_read] = '\0';
      success = (bytes_read > 0); 
      //! If the buffer is full and there is more data, close the request and return a buffer overflow error
      //! The caller is responsible for providing a buffer large enough to hold the response
      //! and to clear the buffer if the request fails
      if (bytes_read >= max_len - 1 && m_http_client.available()) 
      {
        m_http_client.endRequest();
        return NetworkError::BUFFER_OVERFLOW;
      }
    }
    //! End the request
    m_http_client.endRequest();
    m_http_client.stop();
    //! Return the success status
    //! If we get here and we read data, then the request was successful
    //! If we get here and we didn't read data, then the request failed
    return success ? NetworkError::OK : NetworkError::HTTP_ERROR;
  }
};

#endif  // HTTP_BACKEND_NETWORK_H