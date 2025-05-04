#include "mock_io.h"

MockPinIO::MockPinIO(
    std::map<uint8_t, bool> m_digitalReadMap, 
    std::map<uint8_t, uint16_t> m_analogReadMap) :
    m_digitalReadMap(m_digitalReadMap),
    m_analogReadMap(m_analogReadMap)
{}


bool MockPinIO::digitalRead(uint8_t pin)
{
    if (m_digitalReadMap.find(pin) == m_digitalReadMap.end())
    {
        return 0;
    }
    return m_digitalReadMap[pin];
}

void MockPinIO::digitalWrite(uint8_t pin, bool value)
{
    if (m_digitalReadMap.find(pin) == m_digitalReadMap.end())
    {
        return;
    }
    m_digitalReadMap[pin] = value;
}

uint16_t MockPinIO::analogRead(uint8_t pin)
{
    if (m_analogReadMap.find(pin) == m_analogReadMap.end())
    {
        return 0;
    }
    return m_analogReadMap[pin];
}       

void MockPinIO::analogWrite(uint8_t pin, uint16_t value)
{
    if (value > m_analog_resolution)
    {
        value = m_analog_resolution;
    }
    m_analogReadMap[pin] = value;
}   

void MockPinIO::setDigitalRead(uint8_t pin, bool value)
{
    if (m_digitalReadMap.find(pin) == m_digitalReadMap.end())
    {
        return;
    }
    m_digitalReadMap[pin] = value;
}

void MockPinIO::setAnalogRead(uint8_t pin, uint16_t value)
{
    if (value > m_analog_resolution)
    {
        value = m_analog_resolution;
    }
    m_analogReadMap[pin] = value;
}


void MockPinIO::set_analog_resolution(uint16_t resolution)
{
    if (resolution > 1023)
    {
        m_analog_resolution = 4095;
    }
    else
    {
        m_analog_resolution = 1023;
    }
}