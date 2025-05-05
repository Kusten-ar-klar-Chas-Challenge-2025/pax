// pin_config.h
#ifndef CORE_CONFIG_PIN_CONFIG_H
#define CORE_CONFIG_PIN_CONFIG_H

#include <stdint.h> //! uint8_t
#include <stddef.h> //! size_t

// Define Arduino constants for native testing
#ifndef INPUT
#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2
#define HIGH 0x1
#define LOW 0x0
#define A0 14U
#define A1 15U
#define A2 16U
#define A3 17U
#define A4 18U
#define A5 19U
#endif



//! @brief Struct to configure the pins
//! @details This struct is used to configure the pins for the project
struct PinConfig {


    static constexpr uint8_t pin_temp_sensor {A0};
    static constexpr uint8_t pin_debug_led {2};
    static constexpr uint8_t pin_pir_sensor {3};
    static constexpr uint8_t pin_network_select {7};

    //! @brief Struct to store the pin number and mode
    struct PinEntry {
        uint8_t pin;
        uint8_t mode;
    };
    
    //! @brief Compile-time array of pin-mode pairs
    static constexpr PinEntry pins[] = {
        //! @brief Debug LED for room activity
        {pin_debug_led, OUTPUT},
        //! @brief PIR sensor
        {pin_pir_sensor, INPUT},
        //! @brief Network mode selection, LOW for Ethernet, HIGH for WiFi
        {pin_network_select, INPUT_PULLUP},
        //! @brief Temperature sensor
        {pin_temp_sensor, INPUT}
    };

    //! Iterator setup for the pin config array
    //! May have to set pinCount manually if build fails for arduino
    static constexpr size_t pin_count = sizeof(pins) / sizeof(PinEntry);
    using ConstIterator = const PinEntry*;
    static constexpr ConstIterator begin() noexcept { return pins; }
    static constexpr ConstIterator end() noexcept { return pins + pin_count; }
};

#endif