#include <Arduino.h>
#include "arduino_pin_io.h"
#include "pin_config.h"

void ArduinoPinIO::setup()
{
    for (const auto& pin : PinConfig::pins)
    {
        pinMode(pin.pin, pin.mode);
    }
}

bool ArduinoPinIO::digitalRead(uint8_t pin)
{
    return digitalRead(pin);
}

void ArduinoPinIO::digitalWrite(uint8_t pin, bool value)
{
    digitalWrite(pin, value);
}   

uint16_t ArduinoPinIO::analogRead(uint8_t pin)
{
    return analogRead(pin);
}

void ArduinoPinIO::analogWrite(uint8_t pin, uint16_t value)
{
    analogWrite(pin, value);
}

void ArduinoPinIO::pinMode(uint8_t pin, uint8_t mode)
{
    pinMode(pin, mode);
}