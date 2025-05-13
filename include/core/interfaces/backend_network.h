//! @file backend_network.h
//! @author Jennifer Gott
//! @date 2025-05-06
//! @brief Backend network interface
//! Provides the request function to send and recieve HTTP requests and responses.
//! @note This is a pure virtual class, extend this class for your specific backend implementation.

#ifndef CORE_INTERFACES_BACKEND_NETWORK_H
#define CORE_INTERFACES_BACKEND_NETWORK_H

#include <string_view>
#include "network_types.h"

//! @brief HTTP method types
enum class HttpMethod
{
  GET, //!< GET request, used to retrieve data from the server
  POST, //!< POST request, used to send data to the server
  PUT //!< PUT request, used to update data on the server
};

//! @brief Backend network interface
//! @note This is a pure virtual class, extend this class for your specific backend implementation.
class BackendNetwork 
{
  protected:
  BackendNetwork() = default;

  public:

  BackendNetwork(const BackendNetwork&) = delete;
  BackendNetwork& operator=(const BackendNetwork&) = delete;

//! @brief Perform a HTTP request to the backend server
//! @param[in] method HTTP method to use
//! @param[in] endpoint Endpoint to request
//! @param[in] request_body Request body to send, empty for GET requests
//! @param[out] response_buffer Buffer to store the response
//! @param[in] max_len Maximum length of the response buffer
//! @return The error code
//! Usage: auto error = network.request(HttpMethod::GET,  "/api/data",
//!                                     "", 
//!                                     response_buffer, 
//!                                     sizeof(response_buffer));
//! if (is_error(error)) {
//!     // handle the error
//! }
  virtual NetworkError request(HttpMethod method,
                      const std::string_view& endpoint,
                      const std::string_view& request_body,
                      char* response_buffer,
                      size_t max_len) = 0;
  virtual ~BackendNetwork() = default;
};

#endif  // CORE_INTERFACES_BACKEND_NETWORK_H