/**
 * @file PostMan.cpp
 * @brief Handles posting to backend server
 * 
 */
#include "../include/PostMan.h"
#include <ArduinoJson.h>

PostMan::PostMan(const ServerInfo &server_info, NetworkingBase* connection)
    : m_server_info{server_info}, m_connection{connection}, m_has_token{false} {}

/**
 * @brief Create the JSON payload for room state data
 * @param temperature Current room temperature
 * @param occupancyStatus Current room occupancy status
 * @param airQuality Current room air quality
 * @return String JSON payload
 */
String PostMan::createRoomStateJSON(const String &temperature, const String &occupancyStatus, const String &airQuality)
{
    String json = "{\n";
    json += "  \"temperature\": \"" + temperature + "\",\n";    
    json += "  \"activity\": " + occupancyStatus + ",\n";
    json += "  \"air_quality\": \"" + airQuality + "\",\n";
    json += "}";
    return json;
}

/**
 * @brief Send room state update to server
 * @param temperature Current room temperature
 * @param occupancyStatus Current room occupancy status
 * @param airQuality Current room air quality
 * @return true if successful, false otherwise
 */
bool PostMan::sendRoomState(const String &temperature, const String &occupancyStatus, const String &airQuality)
{
    // Ensure we have a valid JWT token
    if (!m_has_token && !getJwtToken()) {
        Serial.println(F("PostMan: Failed to get JWT token for room state update"));
        return false;
    }

    String json = createRoomStateJSON(temperature, occupancyStatus, airQuality);
    String httpRequest = createHTTPHeader(json, "PATCH", String(m_server_info.room_endpoint.data()));
    return sendRequest(httpRequest);
}

/**
 * @brief Send UUID to server to get room ID
 * @param uuid Device UUID
 * @return true if successful, false otherwise
 */
bool PostMan::sendUuid(const std::string_view uuid)
{
    String json = "{\n  \"uuid\": \"" + String(uuid.data()) + "\"\n}";
    String httpRequest = createHTTPHeader(json, "POST", String(m_server_info.config_endpoint.data()));
    return sendRequest(httpRequest);
}

/**
 * @brief Get room ID from server
 * @param room_id Reference to store the room ID
 * @return true if successful, false otherwise
 */
bool PostMan::getRoomId(uint32_t &room_id)
{
    String endpoint = String(m_server_info.config_endpoint.data()) + "/" + String(m_server_info.uuid.data());
    String httpRequest = createHTTPHeader("", "GET", endpoint);
    
    if (!sendRequest(httpRequest)) {
        return false;
    }

    // Get the response from the last request
    String response = "";
    Client* client = m_connection->current_client();
    while (client->available()) {
        response += static_cast<char>(client->read());
    }

    // Find the JSON part of the response (after the HTTP headers)
    int jsonStart = response.indexOf("\r\n\r\n");
    if (jsonStart == -1) {
        Serial.println("No JSON data found in response");
        return false;
    }
    String jsonStr = response.substring(jsonStart + 4);

    // Parse the JSON response
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, jsonStr);

    if (error) {
        Serial.print("JSON parsing failed: ");
        Serial.println(error.c_str());
        return false;
    }

    // Extract room_id from JSON
    if (!doc.containsKey("room_id")) {
        Serial.println("No room_id found in response");
        return false;
    }

    room_id = doc["room_id"].as<uint32_t>();
    return true;
}

/**
 * @brief Create HTTP header with JSON payload
 * @param jsonPayload The JSON data to be sent
 * @param method HTTP method (PUT, GET, etc.)
 * @param endpoint Target endpoint
 * @return String The full HTTP request header
 */
String PostMan::createHTTPHeader(const String &jsonPayload, const String &method, const String &endpoint)
{
    String httpRequest = "";
    httpRequest += method + " " + endpoint + " HTTP/1.1\r\n";
    httpRequest += "Host: " + String(m_server_info.server_base_url.data()) + "\r\n";
    httpRequest += "Content-Type: application/json\r\n";
    httpRequest += "Content-Length: " + String(jsonPayload.length()) + "\r\n";
    
    // Add Authorization header if we have a token and this isn't the login request
    if (m_has_token && endpoint != String(JWT_ENDPOINT)) {
        httpRequest += "Authorization: Bearer " + m_jwt_token + "\r\n";
    }
    
    httpRequest += "Connection: close\r\n\r\n";
    httpRequest += jsonPayload;
    return httpRequest;
}

/**
 * @brief Send HTTP request and handle response
 * @param request The HTTP request to send
 * @return true if successful, false otherwise
 */
bool PostMan::sendRequest(const String &request)
{
    if (!m_connection->ready_for_traffic()) {
        Serial.println(F("PostMan: Network not ready for traffic"));
        return false;
    }

    Client* client = m_connection->current_client();

    // If not connected connect to Server
    if (!client->connected()) {
        Serial.println(F("PostMan: Not connected to server, attempting connection..."));
        if (!m_connection->connect_wifi_to_server()) {
            Serial.println(F("PostMan: Unable to connect to server"));
            return false;
        }
    }

    // Print request details
    Serial.println(F("\nPostMan: Sending HTTP request:"));
    Serial.println(F("----------------------------------------"));
    
    // Construct and print the full URL
    String fullUrl = String(m_server_info.server_base_url.data());
    fullUrl += ":";
    fullUrl += String(m_server_info.server_port);
    
    // Add API path if it exists and doesn't start with a slash
    if (m_server_info.server_api_path.length() > 0) {
        if (m_server_info.server_api_path[0] != '/') {
            fullUrl += "/";
        }
        fullUrl += String(m_server_info.server_api_path.data());
    }
    
    // Extract the endpoint from the first line of the request
    int methodEnd = request.indexOf(" ");
    int httpStart = request.indexOf(" HTTP/");
    if (methodEnd != -1 && httpStart != -1) {
        String endpoint = request.substring(methodEnd + 1, httpStart);
        // Only add a slash if endpoint doesn't start with one and we don't already have a trailing slash
        if (endpoint[0] != '/' && fullUrl[fullUrl.length()-1] != '/') {
            fullUrl += "/";
        }
        fullUrl += endpoint;
    }
    
    Serial.print(F("PostMan: Full URL: "));
    Serial.println(fullUrl);
    Serial.println(F("----------------------------------------"));
    Serial.println(request);
    Serial.println(F("----------------------------------------"));

    // Send the request
    uint16_t charsWritten = client->print(request);
    Serial.print(F("PostMan: Bytes written: "));
    Serial.println(charsWritten);

    // Wait for a response (with a timeout of 5 seconds)
    unsigned long timeout = millis();
    Serial.println(F("PostMan: Waiting for server response..."));
    
    while (client->available() == 0) {
        if (millis() - timeout > 5000) {  // 5000 ms timeout
            Serial.println(F("PostMan: Timeout - No response from server"));
            Serial.print(F("PostMan: Client still connected: "));
            Serial.println(client->connected());
            return false;
        }
        delay(100);  // Small delay to prevent busy waiting
    }

    // Read the server response
    String response = "";
    while (client->available()) {
        char c = client->read();
        response += c;
    }

    // Log server response
    if (response != "") {
        Serial.println(F("\nPostMan: Server response:"));
        Serial.println(F("----------------------------------------"));
        Serial.println(response);
        Serial.println(F("----------------------------------------"));
        
        // Check for HTTP status code
        int statusCode = 0;
        int statusStart = response.indexOf("HTTP/1.1 ");
        if (statusStart != -1) {
            statusCode = response.substring(statusStart + 9, statusStart + 12).toInt();
            Serial.print(F("PostMan: HTTP Status Code: "));
            Serial.println(statusCode);
        }
        
        // Check if the response indicates success
        if (statusCode >= 200 && statusCode < 300) {
            Serial.println(F("PostMan: Request successful"));
        } else {
            Serial.println(F("PostMan: Request failed"));
        }
    } else {
        Serial.println(F("PostMan: Empty response from server"));
    }

    return true;
}

String PostMan::createJwtLoginJSON()
{
    String json = "{\n";
    json += "  \"email\": \"" + String(m_server_info.jwt_user.data()) + "\",\n";
    json += "  \"password\": \"" + String(m_server_info.jwt_pass.data()) + "\"\n";
    json += "}";
    return json;
}

bool PostMan::getJwtToken()
{
    String json = createJwtLoginJSON();
    String httpRequest = createHTTPHeader(json, "POST", String(m_server_info.jwt_endpoint.data()));
    
    if (!sendRequest(httpRequest)) {
        Serial.println(F("PostMan: Failed to send JWT login request"));
        return false;
    }

    // Get the response from the last request
    String response = "";
    Client* client = m_connection->current_client();
    while (client->available()) {
        response += static_cast<char>(client->read());
    }

    // Find the JSON part of the response (after the HTTP headers)
    int jsonStart = response.indexOf("\r\n\r\n");
    if (jsonStart == -1) {
        Serial.println(F("PostMan: No JSON data found in JWT response"));
        return false;
    }
    String jsonStr = response.substring(jsonStart + 4);

    // Parse the JSON response
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, jsonStr);

    if (error) {
        Serial.print(F("PostMan: JWT JSON parsing failed: "));
        Serial.println(error.c_str());
        return false;
    }

    // Extract token from JSON
    if (!doc.containsKey("token")) {
        Serial.println(F("PostMan: No token found in JWT response"));
        return false;
    }

    m_jwt_token = doc["token"].as<String>();
    m_has_token = true;
    Serial.println(F("PostMan: Successfully obtained JWT token"));
    return true;
}