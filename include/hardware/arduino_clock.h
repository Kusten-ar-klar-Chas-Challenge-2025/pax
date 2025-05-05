#ifndef HARDWARE_ARDUINO_CLOCK_H
#define HARDWARE_ARDUINO_CLOCK_H

#include <Arduino.h>
#include <RTC.h>
#include "clock_source.h"

class ArduinoClock : public Clock
{
    public:
    ArduinoClock();
    uint32_t get_time_ms() const noexcept override;
    void set_time() noexcept;
};

#endif

