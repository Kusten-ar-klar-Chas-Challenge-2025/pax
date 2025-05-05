#ifndef CORE_INTERFACES_CLOCK_SOURCE_H
#define CORE_INTERFACES_CLOCK_SOURCE_H

#include <cstdint>

class Clock
{
    protected:
    Clock() = default;
    public:
    virtual uint32_t millis() const noexcept = 0;
    virtual ~Clock() = default;
    virtual void set_time() noexcept = 0;
};



#endif

