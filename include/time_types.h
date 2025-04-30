// time_types.h
#ifndef TIME_TYPES_H
#define TIME_TYPES_H

#include <cstdint>

//! Structure to represent a time window with start and end times
struct time_window_t
{
    uint8_t start_hour;   //!< Starting hour (0-23)
    uint8_t start_minute; //!< Starting minute (0-59)
    uint8_t end_hour;     //!< Ending hour (0-23)
    uint8_t end_minute;   //!< Ending minute (0-59)
};

#endif // TIME_TYPES_H