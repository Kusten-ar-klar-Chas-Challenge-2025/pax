//! @file Backend.h
//! Functions for communicating with the backend server

#ifndef BACKEND_H
#define BACKEND_H

#include <string_view>
#include <array>
#include <cstdint>
#include <WiFiS3.h>
#include "certificates.h"

//! HTTP methods supported by the backend
enum class HttpMethod {
    GET,
    POST,
    PATCH,
    HEAD
};

//! Helper struct for HTTP response
struct HttpResponse {
    int status_code;
    String body;
    bool success;
};

struct ServerInfo {
    //! Base server configuration
    std::string_view server_base_url;    //!< base url of the server
    uint16_t server_port;                //!< port of the server
    std::string_view server_cert = root_cert;  //!< server certificate from PROGMEM
    std::string_view server_api_path;    //!< path to the api

    //! API endpoints - all should start with / and not end with /
    struct {
        std::string_view auth = "/auth";      //!< Base auth endpoint
        std::string_view rooms = "/rooms";    //!< Base rooms endpoint
        std::string_view config = "/config";  //!< Base config endpoint
    } endpoints;

    //! Auth endpoints
    struct {
        std::string_view login = "/login";    //!< Login endpoint (appended to auth)
    } auth;

    //! JWT configuration
    std::string_view jwt_user;           //!< JWT authentication username/email
    std::string_view jwt_pass;           //!< JWT authentication password
    std::string_view uuid;               //!< uuid for the room (128 bit with dashes)
    uint32_t room_id;                    //!< room id for the room (up to 4 bytes)
    
    //! Buffer and view for constructed room endpoint
    std::array<char, 64> room_endpoint_buffer;  //!< Buffer for room endpoint construction
    std::string_view room_endpoint = room_endpoint_buffer.data();             //!< View of current room endpoint
    std::array<char, 512> jwt_token_buffer;      //!< Buffer for JWT token
    std::string_view jwt_token = jwt_token_buffer.data();                 //!< View of current JWT token

    ServerInfo() : 
        server_base_url(""),
        server_port(443),  // Default HTTPS port
        server_api_path(""),
        jwt_user(""),
        jwt_pass(""),
        uuid("00000000-0000-0000-0000-000000000000"),  // Default zero UUID
        room_id(0),  // Default to no room ID
        room_endpoint_buffer(),
        jwt_token_buffer() 
    {}

    //! Constructor that takes all configuration values
    ServerInfo(std::string_view url, uint16_t port, std::string_view api_path,
              std::string_view user, std::string_view pass, std::string_view id) :
        server_base_url(url),
        server_port(port),
        server_api_path(api_path),
        jwt_user(user),
        jwt_pass(pass),
        uuid(id),
        room_id(0),
        room_endpoint_buffer(),
        jwt_token_buffer()
    {}

    void set_jwt_token(std::string_view token);
    void set_room_endpoint(std::string_view endpoint);

    //! Check if the server info is properly configured
    bool is_configured() const {
        return !server_base_url.empty() && 
               server_port != 0 && 
               !server_cert.empty() && 
               !server_api_path.empty() && 
               !jwt_user.empty() && 
               !jwt_pass.empty() && 
               !uuid.empty();
    }
};

class Backend {
public:
    //! Construct a new Backend object
    //! @param server_info Server configuration information data structure
    Backend(ServerInfo& server_info);

    //! Initialize the backend connection
    //! @return true if successful, false otherwise
    bool begin();

    //! Login to get JWT token
    //! @return true if successful, false otherwise
    bool login_jwt();

    //! Update the room ID in the server info
    //! @param room_id The room ID to set
    void update_room_id(uint32_t room_id);

    //! Send room state update to server
    //! @param temperature Current room temperature
    //! @param activity Current room activity status
    //! @param air_quality Current room air quality (integer)
    //! @return true if successful, false otherwise
    bool send_update_room_state(int temperature, bool activity, int air_quality);

    //! Get room configuration from server
    //! @return true if successful, false otherwise
    bool get_room_config();

    //! Check if we have a valid JWT token (internal state only)
    //! @return true if we have a token, false otherwise
    bool has_token() const { return m_has_token; }

    //! Check if we have a valid room ID (internal state only)
    //! @return true if we have a room ID, false otherwise
    bool has_room_id() const { return m_has_room_id; }

    //! Check if the backend is properly configured (internal state only)
    //! @return true if all required configuration is present, false otherwise
    bool is_configured() const { return m_server_info.is_configured(); }

    //! Check if we have an active connection to the server (internal state only)
    //! @return true if connected, false otherwise
    bool is_connected() { return m_client.connected(); }

    //! Check if we can reach the server (makes server contact)
    //! @return true if server is reachable, false otherwise
    bool check_server_connection();

    //! Try to reconnect if connection is down (makes server contact)
    //! @return true if connection is established, false otherwise
    bool ensure_connection();

    //! Check if current JWT token is still valid (makes server contact)
    //! @return true if token is valid, false otherwise
    bool verify_token_validity();

    //! Get the current JWT token
    //! @return string_view of the current JWT token
    std::string_view jwt_token() const { return m_server_info.jwt_token; }

    //! Get the current room endpoint
    //! @return string_view of the current room endpoint
    std::string_view room_endpoint() const { return m_server_info.room_endpoint; }

private:
    //! Helper function to construct API paths
    //! @param dest Destination String to write the path to
    //! @param endpoint Main endpoint (e.g. "auth", "rooms")
    //! @param subpath Optional subpath (e.g. "login", room ID)
    //! @param query Optional query string (e.g. "?param=value")
    //! @return true if path was constructed successfully, false if dest is null or path would be too long
    bool construct_path(String* dest, std::string_view endpoint, std::string_view subpath = "", std::string_view query = "");

    //! Helper function to make HTTP requests
    //! @param method HTTP method to use
    //! @param path Request path
    //! @param headers Additional headers to include (empty string if none)
    //! @param body Request body (empty string if none)
    //! @return HttpResponse containing status code, body, and success flag
    HttpResponse make_http_request(HttpMethod method, std::string_view path, 
                                 std::string_view headers = "", std::string_view body = "");

    //! Helper function to make authenticated HTTP requests
    HttpResponse make_authenticated_request(HttpMethod method, std::string_view path, 
                                          std::string_view headers, std::string_view body);

    ServerInfo& m_server_info;  //!< Server configuration information
    WiFiSSLClient m_client;     //!< SSL client for server communication
    bool m_has_token = false;   //!< Whether we have a valid JWT token
    bool m_has_room_id = false; //!< Whether we have a valid room ID
};

#endif // BACKEND_H