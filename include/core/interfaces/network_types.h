//! @file network_types.h
//! @author Jennifer Gott
//! @date 2025-05-06
//! @brief Network types
//! Used for the network interfaces to define the types used between them.

#ifndef CORE_INTERFACES_NETWORK_TYPES_H
#define CORE_INTERFACES_NETWORK_TYPES_H

#include <cstdint>
#include <string_view>

//! @brief Server configuration for backend communication
struct ServerConfig 
{
  //! @brief Server host address, view into a constant character array
  std::string_view host;
  //! @brief Server port number (default 80)
  uint16_t port {80};
};

//! @brief Network secrets
struct NetworkSecrets 
{
  //! @brief Network SSID, view into a constant character array
  std::string_view ssid;
  //! @brief Network password, view into a constant character array
  std::string_view password;
};

//! @brief Network error codes
//! @note This enum is used to indicate the type of error that occurred during network operations
enum class NetworkError
{
    OK, //!< No error
    NOT_CONNECTED, //!< Not connected to the network
    INVALID_CREDENTIALS, //!< Invalid credentials
    INVALID_RESPONSE, //!< Invalid response
    HTTP_ERROR, //!< HTTP error
    CONNECTION_TIMEOUT, //!< Connection timeout
    NO_BUFFER_PROVIDED, //!< No buffer provided
    NO_REQUEST_MESSAGE, //!< No request message provided
    BUFFER_OVERFLOW, //!< Output buffer can't hold the requested data
    UNKNOWN //!< Unknown error
};

//! @brief Check if the network operation failed
//! @param error The error code to check
//! @return True if the operation failed, false otherwise
//! Usage: if (is_error(error)) {
//!     // handle the error
//! }
inline bool is_error(NetworkError error) {
    return error != NetworkError::OK;
}

#endif // CORE_INTERFACES_NETWORK_TYPES_H