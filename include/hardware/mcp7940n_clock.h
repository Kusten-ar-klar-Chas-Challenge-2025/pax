#include "clock_source.h"
#include <MCP7940.h>

//! @brief MCP7940N clock implementation
//! @note This implementation is based on the Zanduino MCP7940N library
class ClockMcp7940n : public Clock {
    public:
        //! @brief Default constructor
        ClockMcp7940n();
        //! @brief Initialize the MCP7940N clock
        bool begin() { rtc.begin(); }
        //! @brief Get the current time
        //! @return DateTime object containing the current time
        DateTime now() const noexcept override { return rtc.now(); }
        //! @brief Get the current time in milliseconds
        //! @return Time in milliseconds
        //! @note This is the Arduino millis() function and should wrap when the timer overflows
        uint32_t millis() const noexcept override { return ::millis(); }
        //! @brief Set the time on the MCP7940N clock
        //! @param time DateTime object containing the time to set the clock to
        void set_time(DateTime time) noexcept override { rtc.adjust(time); }

        operator bool() const { return rtc.deviceStatus(); }

    private:
        MCP7940_Class rtc;
};