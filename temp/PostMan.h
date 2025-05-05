#ifndef POSTMAN_H
#define POSTMAN_H

#include "networking_base.h"
#include <Arduino.h>
#include <WiFiS3.h>
#include <memory>

class PostMan
{
public:
    PostMan(const char *server, const char *endpoint, uint16_t port, Stream* stream);
    bool sendPost(const String &temperature, const String &occupancyStatus, const String &airQuality);

private:
    const char *server;
    const char *endpoint;
    uint16_t port;
    Stream* m_stream;

    String createJSON(const String &temperature, const String &occupancyStatus, const String &airQuality);
    String createHTTPHeader(const String &jsonPayload);
};

#endif // POSTMAN_H