// system_states.h
#ifndef SYSTEM_STATES_H
#define SYSTEM_STATES_H

#include <cstdint>

//! Enumeration of system states
enum class system_state_t : uint8_t
{
    ACTIVE = 0,     //!< Room occupied, frequent reporting
    TRANSITION = 1, //!< Room just became vacant, holding before slowing down
    IDLE = 2,       //!< Room vacant during office hours, infrequent reporting
    AFTER_HOURS = 3, //!< Outside office hours, minimal operation
    WEEKEND = 4     //!< Weekend mode, very minimal operation
};

//! Number of system states
static constexpr uint8_t STATE_COUNT = 5;

//! State names for configuration and display
static constexpr const char* const STATE_NAMES[] = {
    "ACTIVE",
    "TRANSITION",
    "IDLE",
    "AFTER_HOURS",
    "WEEKEND"
};

//! State names for JSON configuration
static constexpr const char* const STATE_JSON_NAMES[] = {
    "active",
    "transition",
    "idle",
    "after_hours",
    "weekend"
};

#endif // SYSTEM_STATES_H