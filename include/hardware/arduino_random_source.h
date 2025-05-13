#ifndef HARDWARE_ARDUINO_RANDOM_SOURCE_H
#define HARDWARE_ARDUINO_RANDOM_SOURCE_H

#include <Arduino.h>
#include "core/interfaces/random_source.h"

class ArduinoRandomSource : public RandomSource
{
    public:
    void randomSeed(uint32_t seed) override { ::randomSeed(seed); }
    int32_t random(int32_t min, int32_t max) override { return ::random(min, max); }
    int32_t random(int32_t max) override { return ::random(max); }
};

#endif // HARDWARE_ARDUINO_RANDOM_SOURCE_H