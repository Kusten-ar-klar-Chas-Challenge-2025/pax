#ifndef POSTMAN_H
#define POSTMAN_H

#include "networking_base.h"
#include <Arduino.h>

/**
 * @class PostMan
 * @brief A class for packaging the sensor outputs to json format and transmitt via HTTP
 */
class PostMan
{
public:
    /**
    * @brief Determines the server address of the object and what configuration the network is run on
    * 
    * @param serverURL URL of the intended server 
    * @param endpoint endpoint of intended server
    * @param port port of intended server 
    * @param stream ptr to WiFi and Ethernet baseclass
    * 
    * @return true = succesful transmission to server endpoint
    * @return false = failure to send to server endpoint
    */
    PostMan(const char *serverURL, const char *endpoint, uint16_t port, Stream *stream);

    /**
    * @brief Establish a connection and send the HTTP POST request with the JSON payload constructed from the parameters.
    * @param temperature
    * @param occupancyStatus
    * @param airQuality
    * @return true = succesful transmission to server endpoint
    * @return false = failure to send to server endpoint
    */
    bool sendPost(const String &temperature, const String &occupancyStatus, const String &airQuality);
    bool sendPost(const String &temperature, const String &occupancyStatus, const String &airQuality, NetworkingBase &network);

private:
    //! Server URL 
    const char *serverURL;

    //! Server endpoint
    const char *endpoint;
    
    //! Server port 
    uint16_t port;

    //! Pointer to base class for wifi/ethernet
    Stream *m_stream;

    //!
    static constexpr int timeout_SendPost = 5000;

    /**
    * @brief Create the JSON payload that passes sensor data and fetches the current date and time.
    * @param temperature
    * @param occupancyStatus
    * @param airQuality
    * @return String
    */
    String createJSON(const String &temperature, const String &occupancyStatus, const String &airQuality);

    /**
    * @brief Create HTTP POST request header for the given JSON payload.
    * @param jsonPayload The JSON data to be sent.
    * @return String - The full HTTP POST request header.
    */
    String createHTTPHeader(const String &jsonPayload);
};

#endif // POSTMAN_H