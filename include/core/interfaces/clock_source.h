#ifndef CORE_INTERFACES_CLOCK_SOURCE_H
#define CORE_INTERFACES_CLOCK_SOURCE_H

#include <cstdint>

class DateTime; // forward declaration based on RTC library
class TimeSpan; // forward declaration based on RTC library


//! @brief Interface for a clock source
//! Clock should give common interface to time related functions, both 
//! arduino::millis() and RTC time functions.
//! This allows for testing of time related functions without having to depend on the actual time
//! and for use of mock clock sources in tests.
//! Expected clock source implementations:

class Clock
{
    public:
    //! @brief Get time since clock initialized in milliseconds
    //! @return Time in milliseconds
    //! @note This is the Arduino millis() function and should wrap when the timer overflows
    virtual uint32_t millis() const noexcept = 0;
    //! @brief Delay for a given number of milliseconds
    //! @param milliseconds Number of milliseconds to delay
    //! @note This is the Arduino delay() function and should block the calling thread
    virtual void delay(uint32_t milliseconds) const noexcept = 0;
    //! @brief Get the current time
    //! @return DateTime object containing the current time
    //! @note This differs from the R4A1 RTC.h that fills a DateTime object. This usage follows RTClib instead.
    virtual DateTime now() const noexcept = 0;
    //! @brief Set the time on the clock
    //! @param time DateTime object containing the time to set the clock to
    virtual void set_time(const DateTime& time) noexcept = 0;
    //! @brief Destructor for base class
    virtual operator bool() const = 0;
    virtual ~Clock() = default;
    protected:
    //! @brief Default constructor protected to prevent instantiation of abstract class
    Clock() = default;
};



#endif  // CORE_INTERFACES_CLOCK_SOURCE_H

