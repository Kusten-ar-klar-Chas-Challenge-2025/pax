/**
 * @file Backend.cpp
 * 
 */
// Comment this line out to disable verbose debug logging - OR define via build flags
// #define BACKEND_DEBUG 

#include "Backend.h"
#include <Arduino.h>
#include <WiFiS3.h>
#include <ArduinoJson.h>
#include <string_view>
#include <array>
#include <cstdint>
#include <algorithm>

// HTTP header constants
constexpr const char* CONTENT_TYPE_JSON = "Content-Type: application/json\r\n";
constexpr const char* ACCEPT_JSON = "Accept: application/json\r\n";
constexpr const char* USER_AGENT_HEADER = "User-Agent: Arduino/1.0\r\n";

// Composed headers for JSON POST/PATCH requests (excluding login)
constexpr const char* JSON_HEADERS =
    "Content-Type: application/json\r\n"
    "Accept: application/json\r\n"
    "User-Agent: Arduino/1.0\r\n";

// Headers for login POST request - minimal set
constexpr const char* LOGIN_POST_HEADERS =
    "Content-Type: application/json\r\n";

// Composed headers
constexpr const char* ACCEPT_HEADERS =
    "Accept: application/json\r\n"
    "User-Agent: Arduino/1.0\r\n";

//! Set the JWT token in the buffer
void ServerInfo::set_jwt_token(std::string_view token) {
    size_t len = std::min(token.size(), jwt_token_buffer.size() - 1);
    std::copy(token.begin(), token.begin() + len, jwt_token_buffer.begin());
    jwt_token_buffer[len] = '\0';
    jwt_token = std::string_view(jwt_token_buffer.data(), len);
}

//! Set the room endpoint in the buffer
void ServerInfo::set_room_endpoint(std::string_view endpoint) {
    size_t len = std::min(endpoint.size(), room_endpoint_buffer.size() - 1);
    std::copy(endpoint.begin(), endpoint.begin() + len, room_endpoint_buffer.begin());
    room_endpoint_buffer[len] = '\0';
    room_endpoint = std::string_view(room_endpoint_buffer.data(), len);
}

Backend::Backend(ServerInfo& server_info) : m_server_info(server_info) {}

bool Backend::construct_path(String* dest, std::string_view endpoint, std::string_view subpath, std::string_view query) {
    if (!dest) {
        return false;
    }

    // Calculate maximum possible length
    size_t max_len = 256;  // Reasonable max path length
    size_t needed_len = 0;
    
    // Add lengths of all components
    if (!m_server_info.server_api_path.empty()) {
        needed_len += m_server_info.server_api_path.length() + 1;  // +1 for potential slash
    }
    if (!endpoint.empty()) {
        needed_len += endpoint.length() + 1;  // +1 for potential slash
    }
    if (!subpath.empty()) {
        needed_len += subpath.length() + 1;  // +1 for potential slash
    }
    if (!query.empty()) {
        needed_len += query.length();
    }
    
    // Check if path would be too long
    if (needed_len > max_len) {
        return false;
    }
    
    // Reserve space in destination
    dest->reserve(needed_len);
    dest->remove(0);  // Clear the string
    
    // Start with API path if not empty
    if (!m_server_info.server_api_path.empty()) {
        *dest = m_server_info.server_api_path.data();
        if (!dest->endsWith("/")) {
            *dest += "/";
        }
    }
    
    // Add endpoint if provided
    if (!endpoint.empty()) {
        // If endpoint starts with slash, use it directly
        if (endpoint[0] == '/') {
            *dest += endpoint.data();
        } else {
            // Otherwise ensure we have a slash
            if (!dest->endsWith("/")) {
                *dest += "/";
            }
            *dest += endpoint.data();
        }
    }
    
    // Add subpath if provided
    if (!subpath.empty()) {
        // If subpath starts with slash, use it directly
        if (subpath[0] == '/') {
            *dest += subpath.data();
        } else {
            // Otherwise ensure we have a slash
            if (!dest->endsWith("/")) {
                *dest += "/";
            }
            *dest += subpath.data();
        }
    }
    
    // Add query if provided
    if (!query.empty()) {
        *dest += query.data();
    }
    
    return true;
}

bool Backend::begin() {
    // Set root certificate for SSL
    if (m_server_info.server_cert.empty()) {
#ifdef BACKEND_DEBUG
        Serial.println(F("Backend: Error - Server certificate is empty"));
#endif
        return false;
    }
#ifdef BACKEND_DEBUG
    Serial.println(F("Backend: Setting SSL certificate..."));
#endif
    m_client.setCACert(m_server_info.server_cert.data());
    
    // Connect to server with retries
#ifdef BACKEND_DEBUG
    Serial.print(F("Backend: Connecting to "));
    Serial.print(m_server_info.server_base_url.data());
    Serial.print(F(":"));
    Serial.println(m_server_info.server_port);
#endif
    
    const size_t max_retries = 3;
    const time_t retry_delay = 1000;
    
    for (size_t i = 0; i < max_retries; i++) {
        if (m_client.connect(m_server_info.server_base_url.data(), m_server_info.server_port)) {
            // Wait for connection to stabilize
            delay(500);
            if (m_client.connected()) {
#ifdef BACKEND_DEBUG
                Serial.println(F("Backend: Successfully connected to server"));
#endif
                break;
            }
        }
        
        if (i < max_retries - 1) {
#ifdef BACKEND_DEBUG
            Serial.print(F("Backend: Connection attempt "));
            Serial.print(i + 1);
            Serial.print(F(" failed, retrying in "));
            Serial.print(retry_delay);
            Serial.println(F("ms..."));
#endif
            delay(retry_delay);
        } else {
#ifdef BACKEND_DEBUG
            Serial.println(F("Backend: Error - Failed to connect to server after all retries"));
#endif
            return false;
        }
    }
    
    // Get JWT token if we don't have one
    if (!m_has_token) {
#ifdef BACKEND_DEBUG
        Serial.println(F("Backend: No JWT token, attempting login..."));
#endif
        if (!login_jwt()) {
#ifdef BACKEND_DEBUG
            Serial.println(F("Backend: Error - Failed to get JWT token"));
#endif
            return false;
        }
#ifdef BACKEND_DEBUG
        Serial.println(F("Backend: Successfully obtained JWT token"));
#endif
    }
    
    return true;
}

// Helper function to make HTTP requests
HttpResponse Backend::make_http_request(HttpMethod method, std::string_view path, 
                                      std::string_view headers, std::string_view body) {
    HttpResponse response = {-1, "", false};
    
    // Convert method to string
    const char* method_str;
    switch (method) {
        case HttpMethod::GET:    method_str = "GET"; break;
        case HttpMethod::POST:   method_str = "POST"; break;
        case HttpMethod::PATCH:  method_str = "PATCH"; break;
        case HttpMethod::HEAD:   method_str = "HEAD"; break;
        default: 
            response.body = "Invalid HTTP method";
            return response;
    }
    
    // Ensure we have a valid connection before sending request
    if (!ensure_connection()) {
        response.body = "No valid connection for HTTP request";
#ifdef BACKEND_DEBUG
        Serial.println(F("Backend: Error - No valid connection for HTTP request"));
#endif
        return response;
    }
    
    // Verify connection is still valid
    if (!m_client.connected()) {
        response.body = "Lost connection before sending request";
#ifdef BACKEND_DEBUG
        Serial.println(F("Backend: Error - Lost connection before sending request"));
#endif
        return response;
    }
    
    // Construct request
    char request_buffer[1024];
    int content_length = body.empty() ? 0 : body.length();
    
    // Ensure path starts with a slash
    String path_str = path.data();
    if (!path_str.startsWith("/")) {
        path_str = "/" + path_str;
    }
    
    // Print request details before sending (original debug block, still useful)
#ifdef BACKEND_DEBUG
    Serial.print(F("Backend: HTTP Request Details (prior to buffer construction):"));
    Serial.print(F("  Method: "));
    Serial.println(method_str);
    Serial.print(F("  Path: "));
    Serial.println(path_str);
    Serial.print(F("  Host: "));
    Serial.println(m_server_info.server_base_url.data()); // string_view data
    Serial.print(F("  Additional Headers (raw input):"));
    Serial.write(headers.data(), headers.length()); // Print string_view safely
    Serial.println(); // Newline after headers
    Serial.println(F("  Connection: close"));
    if (content_length > 0) {
        Serial.print(F("  Content-Length: "));
        Serial.println(content_length);
        Serial.print(F("  Body (raw input, length "));
        Serial.print(body.length());
        Serial.print(F("):"));
        Serial.write(body.data(), body.length()); // Print string_view safely
        Serial.println(); // Newline after body
    }
    Serial.println(F("--- End of Request Details ---"));
#endif
    
    int offset = 0;
    int written_this_call = 0;
    bool buffer_ok = true;
    int written = 0; // Clear written before new construction logic

    // Request Line (Method Path HTTP/Version)
    if (buffer_ok) {
        written_this_call = snprintf(request_buffer + offset, sizeof(request_buffer) - offset,
                                     "%s %s HTTP/1.1\r\n", method_str, path_str.c_str());
        if (written_this_call < 0) { buffer_ok = false; Serial.println(F("snprintf error R1")); }
        else if (static_cast<size_t>(offset + written_this_call) >= sizeof(request_buffer)) { buffer_ok = false; Serial.println(F("snprintf overflow R1")); }
        if (buffer_ok) offset += written_this_call;
    }

    // Host Header
    if (buffer_ok) {
        written_this_call = snprintf(request_buffer + offset, sizeof(request_buffer) - offset,
                                     "Host: %.*s\r\n",
                                     static_cast<int>(m_server_info.server_base_url.length()),
                                     m_server_info.server_base_url.data());
        if (written_this_call < 0) { buffer_ok = false; Serial.println(F("snprintf error H1")); }
        else if (static_cast<size_t>(offset + written_this_call) >= sizeof(request_buffer)) { buffer_ok = false; Serial.println(F("snprintf overflow H1")); }
        if (buffer_ok) offset += written_this_call;
    }

    // Additional Headers (from input 'headers' string_view)
    if (buffer_ok && !headers.empty()) {
        written_this_call = snprintf(request_buffer + offset, sizeof(request_buffer) - offset,
                                     "%.*s", // 'headers' (e.g. LOGIN_POST_HEADERS) should provide its own trailing \r\n
                                     static_cast<int>(headers.length()),
                                     headers.data());
        if (written_this_call < 0) { buffer_ok = false; Serial.println(F("snprintf error AH1")); }
        else if (static_cast<size_t>(offset + written_this_call) >= sizeof(request_buffer)) { buffer_ok = false; Serial.println(F("snprintf overflow AH1")); }
        if (buffer_ok) offset += written_this_call;
    }
    
    // Connection: close Header
    if (buffer_ok) {
        written_this_call = snprintf(request_buffer + offset, sizeof(request_buffer) - offset,
                                     "Connection: close\r\n");
        if (written_this_call < 0) { buffer_ok = false; Serial.println(F("snprintf error C1")); }
        else if (static_cast<size_t>(offset + written_this_call) >= sizeof(request_buffer)) { buffer_ok = false; Serial.println(F("snprintf overflow C1")); }
        if (buffer_ok) offset += written_this_call;
    }

    // Content-Length Header (if body exists)
    if (buffer_ok && content_length > 0) {
        written_this_call = snprintf(request_buffer + offset, sizeof(request_buffer) - offset,
                                     "Content-Length: %d\r\n", content_length);
        if (written_this_call < 0) { buffer_ok = false; Serial.println(F("snprintf error CL1")); }
        else if (static_cast<size_t>(offset + written_this_call) >= sizeof(request_buffer)) { buffer_ok = false; Serial.println(F("snprintf overflow CL1")); }
        if (buffer_ok) offset += written_this_call;
    }

    // Separator line (\r\n) between headers and body
    if (buffer_ok) {
        written_this_call = snprintf(request_buffer + offset, sizeof(request_buffer) - offset, "\r\n");
        if (written_this_call < 0) { buffer_ok = false; Serial.println(F("snprintf error S1")); }
        else if (static_cast<size_t>(offset + written_this_call) >= sizeof(request_buffer)) { buffer_ok = false; Serial.println(F("snprintf overflow S1")); }
        if (buffer_ok) offset += written_this_call;
    }

    // Body (if exists)
    if (buffer_ok && content_length > 0 && !body.empty()) {
        written_this_call = snprintf(request_buffer + offset, sizeof(request_buffer) - offset,
                                     "%.*s",
                                     static_cast<int>(body.length()),
                                     body.data());
        if (written_this_call < 0) { buffer_ok = false; Serial.println(F("snprintf error B1")); }
        else if (static_cast<size_t>(offset + written_this_call) >= sizeof(request_buffer)) { buffer_ok = false; Serial.println(F("snprintf overflow B1")); }
        if (buffer_ok) offset += written_this_call;
    }

    if (!buffer_ok) {
        response.body = "Request buffer overflow during construction";
#ifdef BACKEND_DEBUG
        Serial.println(F("Backend: Error - Request buffer overflow during construction"));
#endif
        // Note: original code didn't stop m_client here for this specific error.
        return response; 
    }

    written = offset; // 'written' now holds the total length of the constructed request string.
    // request_buffer is null-terminated by the last successful snprintf call.

    // Debug: Print the fully constructed request_buffer
#ifdef BACKEND_DEBUG
    Serial.println(F("Backend: Fully constructed request_buffer (first 256 chars):"));
    char temp_print_buf[257]; // Print a manageable chunk
    strncpy(temp_print_buf, request_buffer, 256);
    temp_print_buf[256] = '\0';
    Serial.println(temp_print_buf);
    if (written > 256) {
      Serial.println(F("... (request truncated for print) ..."));
    }
    Serial.print(F("Backend: Total length of constructed request: "));
    Serial.println(written);
    Serial.println(F("--- End of constructed request_buffer ---"));
#endif
    
    // Send request
    size_t bytes_written = m_client.print(request_buffer); // m_client.print() on char* will send until null.
                                                           // Or use m_client.write(request_buffer, written) to send exact length.
                                                           // Let's use write for precision with 'written' length.
    // size_t bytes_written = m_client.write(reinterpret_cast<const uint8_t*>(request_buffer), written);


    if (bytes_written == 0 && written > 0) { // check written > 0 to ensure we actually tried to write something.
        response.body = "Failed to write request to client";
#ifdef BACKEND_DEBUG
        Serial.println(F("Backend: Error - Failed to write request to client"));
#endif
        return response;
    }
    
    // Verify connection is still valid after sending
    if (!m_client.connected()) {
        response.body = "Lost connection after sending request";
#ifdef BACKEND_DEBUG
        Serial.println(F("Backend: Error - Lost connection after sending request"));
#endif
        return response;
    }
    
    // Wait for initial response with timeout
#ifdef BACKEND_DEBUG
    Serial.println(F("Backend: Waiting for response..."));
#endif
    const unsigned long response_timeout = 5000;  // 5 second timeout
    const unsigned long start_time = millis();
    bool response_started = false;
    
    while (millis() - start_time < response_timeout) {
        if (m_client.available()) {
            response_started = true;
            break;
        }
        delay(50);  // Small delay to prevent tight loop
    }
    
    if (!response_started) {
        response.body = "No response received within timeout";
#ifdef BACKEND_DEBUG
        Serial.println(F("Backend: Error - No response received within timeout"));
        Serial.print(F("Backend: Connection status: "));
        Serial.println(m_client.connected() ? "connected" : "disconnected");
#endif
        return response;
    }
    
    // Read response with timeout
    unsigned long timeout = millis() + response_timeout;
    char buffer[512];
    int bytesRead;
    int totalBytesRead = 0;
    int contentLength = -1;
    bool foundContentLength = false;
    String status_line;
    bool got_status_line = false;
    
    // First pass: read headers
    while (millis() < timeout) {
        if (m_client.available()) {
            bytesRead = m_client.readBytesUntil('\n', buffer, sizeof(buffer) - 1);
            buffer[bytesRead] = '\0';
            
            // Extract status code from first line
            if (!got_status_line) {
                status_line = String(buffer);
                if (status_line.indexOf("HTTP/1.1") == 0) {
                    response.status_code = status_line.substring(9, 12).toInt();
                    got_status_line = true;
#ifdef BACKEND_DEBUG
                    Serial.print(F("Backend: Response status: "));
                    Serial.println(status_line);
#endif
                } else {
                    response.body = "Invalid HTTP response: " + status_line;
#ifdef BACKEND_DEBUG
                    Serial.print(F("Backend: Error - Invalid HTTP response: "));
                    Serial.println(status_line);
#endif
                    return response;
                }
            }
            
            // Look for Content-Length header
            if (!foundContentLength && strncmp(buffer, "Content-Length:", 14) == 0) {
                contentLength = atoi(buffer + 15);
                foundContentLength = true;
            }
            
            // Check for end of headers
            if (bytesRead <= 2 && (buffer[0] == '\r' || buffer[0] == '\n')) {
                break;
            }
            
            totalBytesRead += bytesRead;
        }
        delay(10);
    }
    
    if (!got_status_line) {
        response.body = "Failed to get HTTP status line";
#ifdef BACKEND_DEBUG
        Serial.println(F("Backend: Error - Failed to get HTTP status line"));
#endif
        return response;
    }
    
    // Second pass: read body if needed
    if (foundContentLength && contentLength > 0) {
#ifdef BACKEND_DEBUG
        Serial.println(F("Backend: Reading response body..."));
#endif
        int bodyBytesRead = 0;
        unsigned long body_timeout = millis() + response_timeout;
        
        while (bodyBytesRead < contentLength && millis() < body_timeout) {
            if (m_client.available()) {
                bytesRead = m_client.readBytes(buffer, min(sizeof(buffer) - 1, contentLength - bodyBytesRead));
                buffer[bytesRead] = '\0';
                response.body += String(buffer);
                bodyBytesRead += bytesRead;
            }
            delay(10);
        }
        
        if (bodyBytesRead < contentLength) {
            response.body += "\nWarning: Incomplete body received";
#ifdef BACKEND_DEBUG
            Serial.println(F("Backend: Warning - Incomplete body received"));
            Serial.print(F("Backend: Connection status: "));
            Serial.println(m_client.connected() ? "connected" : "disconnected");
#endif
        }
    }
    
    // If server already closed it, m_client.connected() would be false.
    // Calling stop() is generally safe for WiFiSSLClient even if already stopped, and ensures cleanup.
#ifdef BACKEND_DEBUG
    if (m_client.connected()) {
        Serial.println(F("Backend: make_http_request - Client still connected, explicitly stopping."));
    } else {
        Serial.println(F("Backend: make_http_request - Client was already disconnected (e.g. by server due to 'Connection: close')."));
    }
    // Always call stop() to ensure client resources are cleaned up before the next transaction.
    Serial.println(F("Backend: make_http_request - Calling m_client.stop() for cleanup."));
#endif
    m_client.stop();

    response.success = response.status_code >= 200 && response.status_code < 300;
    return response;
}

bool Backend::check_server_connection() {
    if (!m_client.connected()) {
#ifdef BACKEND_DEBUG
        Serial.println(F("Backend: Not connected to server"));
#endif
        return false;
    }
    
    HttpResponse response = make_http_request(HttpMethod::HEAD, m_server_info.server_api_path, "", "");
    return response.success;
}

bool Backend::ensure_connection() {
    if (m_client.connected()) {
        return true;
    }
    
    // Try to reconnect with retries
    const size_t max_retries = 3;
    const time_t retry_delay = 1000;
    bool connected_in_loop = false; 
    
    for (size_t i = 0; i < max_retries; i++) {
        if (m_client.connect(m_server_info.server_base_url.data(), m_server_info.server_port)) {
            // Wait for connection to stabilize
            delay(500);
            if (m_client.connected()) {
#ifdef BACKEND_DEBUG
                Serial.println(F("Backend: Successfully reconnected to server"));
#endif
                connected_in_loop = true;
                break; 
            }
        }
        
        if (i < max_retries - 1) {
#ifdef BACKEND_DEBUG
            Serial.print(F("Backend: Reconnection attempt "));
            Serial.print(i + 1);
            Serial.print(F(" failed, retrying in "));
            Serial.print(retry_delay);
            Serial.println(F("ms..."));
#endif
            delay(retry_delay);
        }
    } // End of for loop
    
    if (!connected_in_loop) { // If loop finished without connecting
#ifdef BACKEND_DEBUG
        Serial.println(F("Backend: Error - Failed to reconnect to server after all retries"));
#endif
        return false; // Failed to connect
    }
    
    // If we reach here, connected_in_loop is true, meaning connection was successful.
    // The recursive call to verify_token_validity() was here and has been removed.
    
    return true; // Successfully connected (or reconnected)
}

bool Backend::verify_token_validity() {
    if (!m_has_token) {
        return false;
    }
    
    String path;
    if (!construct_path(&path, m_server_info.endpoints.rooms)) {
#ifdef BACKEND_DEBUG
        Serial.println(F("Backend: Failed to construct path"));
#endif
        return false;
    }
    
    HttpResponse response = make_authenticated_request(HttpMethod::GET, path.c_str(), ACCEPT_HEADERS, "");
    
    if (!response.success) {
        if (response.status_code == 401) {
#ifdef BACKEND_DEBUG
            Serial.println(F("Backend: Token validation failed - Unauthorized"));
#endif
            m_has_token = false;
        } else {
#ifdef BACKEND_DEBUG
            Serial.print(F("Backend: Token validation failed - Unexpected status: "));
            Serial.println(response.status_code);
#endif
        }
        return false;
    }
    
    return true;
}

bool Backend::login_jwt() {
    // DEBUG: Print current JWT user and pass as seen by Backend
    // These lines have been REMOVED to prevent accidental credential leakage
    // Serial.print(F("Backend::login_jwt() - m_server_info.jwt_user: "));
    // Serial.write(m_server_info.jwt_user.data(), m_server_info.jwt_user.length());
    // Serial.println();
    // Serial.print(F("Backend::login_jwt() - m_server_info.jwt_pass: "));
    // Serial.write(m_server_info.jwt_pass.data(), m_server_info.jwt_pass.length());
    // Serial.println();

    // Create JSON payload
    StaticJsonDocument<512> doc;
    doc["email"] = m_server_info.jwt_user;
    doc["password"] = m_server_info.jwt_pass;
    
    char json_buffer[512]; // Temporary char buffer
    size_t actual_serialization_len = serializeJson(doc, json_buffer, sizeof(json_buffer)); // Serialize to the buffer
    
    String json = json_buffer; // Create String from buffer

    // DEBUG: Print the generated JSON string and its length
#ifdef BACKEND_DEBUG
    Serial.print(F("Backend::login_jwt() - Serialized to buffer (len from serializeJson): "));
    Serial.println(actual_serialization_len);
    Serial.print(F("Backend::login_jwt() - String from buffer: "));
    Serial.println(json);
    Serial.print(F("Backend::login_jwt() - String from buffer length: "));
    Serial.println(json.length());
#endif
    
    // Try with relative path first
    String path;
    if (!construct_path(&path, m_server_info.endpoints.auth, m_server_info.auth.login)) {
#ifdef BACKEND_DEBUG
        Serial.println(F("Backend: Failed to construct relative path"));
#endif
        return false;
    }
#ifdef BACKEND_DEBUG
    Serial.println(F("Backend: Sending JWT login request..."));
    Serial.print(F("Backend: Request URL (relative): "));
    Serial.println(path);
    Serial.print(F("Backend: Server URL: "));
    Serial.println(m_server_info.server_base_url.data());
#endif
    
    // Use LOGIN_POST_HEADERS for the login request
    HttpResponse response = make_http_request(HttpMethod::POST, path.c_str(),
        LOGIN_POST_HEADERS,
        json.c_str());
    
    // If relative path fails, try absolute path
    if (!response.success && response.status_code == 400) {
#ifdef BACKEND_DEBUG
        Serial.println(F("Backend: Relative path failed, trying absolute path..."));
#endif
        
        // Construct path components
        String path_components;
        path_components.reserve(256);
        
        // Add API path if not empty
        if (!m_server_info.server_api_path.empty()) {
            String api_path = m_server_info.server_api_path.data();
            // Remove leading and trailing slashes from API path
            while (api_path.startsWith("/")) {
                api_path.remove(0, 1);
            }
            while (api_path.endsWith("/")) {
                api_path.remove(api_path.length() - 1);
            }
            if (!api_path.isEmpty()) {
                path_components += "/";
                path_components += api_path;
            }
        }
        
        // Add auth endpoint
        String auth_endpoint = m_server_info.endpoints.auth.data();
        // Remove leading and trailing slashes from auth endpoint
        while (auth_endpoint.startsWith("/")) {
            auth_endpoint.remove(0, 1);
        }
        while (auth_endpoint.endsWith("/")) {
            auth_endpoint.remove(auth_endpoint.length() - 1);
        }
        if (!auth_endpoint.isEmpty()) {
            path_components += "/";
            path_components += auth_endpoint;
        }
        
        // Add login endpoint
        String login_endpoint = m_server_info.auth.login.data();
        // Remove leading and trailing slashes from login endpoint
        while (login_endpoint.startsWith("/")) {
            login_endpoint.remove(0, 1);
        }
        while (login_endpoint.endsWith("/")) {
            login_endpoint.remove(login_endpoint.length() - 1);
        }
        if (!login_endpoint.isEmpty()) {
            path_components += "/";
            path_components += login_endpoint;
        }
        
        // Ensure path starts with a slash
        if (!path_components.startsWith("/")) {
            path_components = "/" + path_components;
        }
        
#ifdef BACKEND_DEBUG
        Serial.print(F("Backend: Request path: "));
        Serial.println(path_components);
        Serial.print(F("Backend: Full request details:\n"));
        Serial.print(F("  Method: POST\n"));
        Serial.print(F("  Host: "));
        Serial.println(m_server_info.server_base_url.data());
        Serial.print(F("  Path: "));
        Serial.println(path_components);
        Serial.print(F("  Headers:\n"));
        Serial.print(F("    Content-Type: application/json\n"));
        Serial.print(F("    Accept: application/json\n"));
        Serial.print(F("    Connection: close\n"));
        Serial.print(F("    Content-Length: "));
        Serial.println(json.length());
#endif
        
        // Use LOGIN_POST_HEADERS for the login request (absolute path attempt)
        response = make_http_request(HttpMethod::POST, path_components.c_str(),
            LOGIN_POST_HEADERS,
            json.c_str());
    }
    
    if (!response.success) {
        if (response.status_code == 401) {
#ifdef BACKEND_DEBUG
            Serial.println(F("Backend: Error - Invalid credentials (401 Unauthorized)"));
#endif
        } else {
#ifdef BACKEND_DEBUG
            Serial.print(F("Backend: Error - Unexpected status code: "));
            Serial.println(response.status_code);
            Serial.print(F("Backend: Response details:\n"));
            Serial.print(F("  Status: "));
            Serial.println(response.status_code);
            Serial.print(F("  Success: "));
            Serial.println(response.success ? "true" : "false");
            Serial.print(F("  Body length: "));
            Serial.println(response.body.length());
#endif
        }
        m_has_token = false;
        return false;
    }
    
    // Parse JSON response
    StaticJsonDocument<512> response_doc;
    DeserializationError error = deserializeJson(response_doc, response.body);
    
    if (error) {
#ifdef BACKEND_DEBUG
        Serial.print(F("Backend: JSON parse error: "));
        Serial.println(error.c_str());
#endif
        return false;
    }
    
    if (response_doc.containsKey("token")) {
        const char* token = response_doc["token"].as<const char*>();
        m_server_info.set_jwt_token(std::string_view(token));
        m_has_token = true;
#ifdef BACKEND_DEBUG
        Serial.println(F("Backend: Successfully parsed and stored JWT token"));
#endif
        return true;
    }
    
#ifdef BACKEND_DEBUG
    Serial.println(F("Backend: Error - No token in response"));
    Serial.print(F("Backend: Response keys: "));
    for (JsonPair kv : response_doc.as<JsonObject>()) {
        Serial.print(kv.key().c_str());
        Serial.print(F(" "));
    }
    Serial.println();
#endif
    return false;
}

void Backend::update_room_id(uint32_t room_id) {
    m_server_info.room_id = room_id;
    String endpoint = String(m_server_info.endpoints.rooms.data()) + "/" + String(room_id);
    m_server_info.set_room_endpoint(std::string_view(endpoint.c_str(), endpoint.length()));
    m_has_room_id = true;
}

bool Backend::send_update_room_state(int temperature, bool activity, int air_quality) {
    if (!m_has_token || !m_has_room_id) {
        return false;
    }
    
    // Ensure we have a valid connection
    if (!ensure_connection()) {
        return false;
    }
    
    // Create JSON payload
    StaticJsonDocument<512> doc;
    doc["temperature"] = temperature;
    doc["activity"] = activity;
    doc["air_quality"] = air_quality;
    
    String json;
    serializeJson(doc, json);
    
    String path;
    if (!construct_path(&path, m_server_info.endpoints.rooms, 
                       String(m_server_info.room_id).c_str())) {
#ifdef BACKEND_DEBUG
        Serial.println(F("Backend: Failed to construct path"));
#endif
        return false;
    }
    
    HttpResponse response = make_authenticated_request(HttpMethod::PATCH, path.c_str(), 
        JSON_HEADERS, json.c_str());
    
    if (!response.success) {
        if (response.status_code == 401) {
#ifdef BACKEND_DEBUG
            Serial.println(F("Backend: Update failed - Unauthorized"));
#endif
            m_has_token = false;
        } else if (response.status_code == 404) {
#ifdef BACKEND_DEBUG
            Serial.println(F("Backend: Update failed - Room not found"));
#endif
            m_has_room_id = false;
            update_room_id(0);
        } else {
#ifdef BACKEND_DEBUG
            Serial.print(F("Backend: Update failed - Unexpected status: "));
            Serial.println(response.status_code);
            Serial.print(F("Backend: Response body: "));
            Serial.println(response.body);
#endif
        }
        return false;
    }
    
    return true;
}

bool Backend::get_room_config() {
    if (!m_has_token) {
        return false;
    }
    
    // Ensure we have a valid connection
    if (!ensure_connection()) {
        return false;
    }
    
    String path;
    if (!construct_path(&path, m_server_info.endpoints.config, m_server_info.uuid.data())) {
#ifdef BACKEND_DEBUG
        Serial.println(F("Backend: Failed to construct path"));
#endif
        return false;
    }
    
    HttpResponse response = make_authenticated_request(HttpMethod::GET, path.c_str(), ACCEPT_HEADERS, "");
    
    // Parse JSON response
    StaticJsonDocument<512> doc;
    DeserializationError error;
    
    switch (response.status_code) {
        case 200:  // Success
            error = deserializeJson(doc, response.body);
            if (error) {
#ifdef BACKEND_DEBUG
                Serial.print(F("Backend: JSON parse error: "));
                Serial.println(error.c_str());
#endif
                update_room_id(0);
                return true;  // Still return true as this is valid for unconfigured UUID
            }
            
            if (doc.containsKey("room_id")) {
                update_room_id(doc["room_id"].as<uint32_t>());
            } else {
                update_room_id(0);
            }
            return true;
            
        case 404:  // Not Found - Valid for unconfigured UUID
#ifdef BACKEND_DEBUG
            Serial.println(F("Backend: Room not found - Valid for unconfigured UUID"));
#endif
            update_room_id(0);
            return true;
            
        case 401:  // Unauthorized
#ifdef BACKEND_DEBUG
            Serial.println(F("Backend: Config request failed - Unauthorized"));
#endif
            m_has_token = false;
            return false;
            
        default:  // Any other status code is an error
#ifdef BACKEND_DEBUG
            Serial.print(F("Backend: Config request failed - Unexpected status: "));
            Serial.println(response.status_code);
            Serial.print(F("Backend: Response body: "));
            Serial.println(response.body);
#endif
            return false;
    }
}

// Helper function to make authenticated HTTP requests
HttpResponse Backend::make_authenticated_request(HttpMethod method, std::string_view path, 
                                               std::string_view headers, std::string_view body) {
    if (!m_has_token || m_server_info.jwt_token.empty()) {
#ifdef BACKEND_DEBUG
        Serial.println(F("Backend: Error - No valid token for authenticated request"));
#endif
        return {-1, "Error: No valid token", false};
    }
    
    // Add Authorization header
    char auth_header[256];
    int auth_written = snprintf(auth_header, sizeof(auth_header),
        "Authorization: Bearer %.*s\r\n",
        static_cast<int>(m_server_info.jwt_token.length()), m_server_info.jwt_token.data());
    
    if (auth_written >= sizeof(auth_header)) {
#ifdef BACKEND_DEBUG
        Serial.println(F("Backend: Error - Auth header buffer overflow"));
#endif
        return {-1, "Error: Auth header buffer overflow", false};
    }
    
    // Combine headers
    String combined_headers = String(auth_header) + headers.data();
    
    return make_http_request(method, path, combined_headers.c_str(), body);
}


