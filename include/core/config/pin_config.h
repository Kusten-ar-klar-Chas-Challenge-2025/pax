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

    enum class Pin : uint8_t {
        UART_TX, //!< UART TX pin
        UART_RX, //!< UART RX pin
        DEBUG_LED, //!< Debug LED, digital pin
        PIR_SENSOR, //!< PIR sensor, digital pin (PWM capable)
        SD_CARD_CS, //!< SD card chip select (on the Ethernet shield)
        DIGITAL_PIN_5, //!< Digital pin 5
        DIGITAL_PIN_6, //!< Digital pin 6
        NETWORK_SELECT, //!< Network mode selection, digital pin
        DIGITAL_PIN_8, //!< Digital pin 8
        DIGITAL_PIN_9, //!< Digital pin 9
        SPI_CS, //!< SPI chip select, Ethernet shield CS
        SPI_COPI, //!< SPI controller out/peripheral in
        SPI_CIPO, //!< SPI controller in/peripheral out
        SPI_CLOCK, //!< SPI clock signal
        TEMP_SENSOR, //!< Temperature sensor, analog pin
        ANALOG_PIN_A1, //!< Unused analog pin
        ENTROPY_SOURCE, //!< Pin used as entropy source for randomSeed()
        ANALOG_PIN_A3, //!< Unused analog pin
        I2C_SDA, //!< I2C data pin
        I2C_SCL, //!< I2C clock pin
    };

    //! @brief Struct to store the pin number and mode
    struct PinEntry {
        Pin pin;
        uint8_t mode;
    };
    
    //! @brief Compile-time array of pin-mode pairs for setting pin mode
    //! @note INPUT is default mode of all pins, so we don't really need to set it, but its here for clarity
    static constexpr PinEntry pins[] = {
        //! @brief Debug LED for room activity
        {Pin::DEBUG_LED, OUTPUT},
        //! @brief PIR sensor
        {Pin::PIR_SENSOR, INPUT},
        //! @brief Network mode selection, LOW for Ethernet, HIGH for WiFi
        {Pin::NETWORK_SELECT, INPUT_PULLUP},
        //! @brief Temperature sensor
        {Pin::TEMP_SENSOR, INPUT}
    };

    //! Iterator setup for the pin config array
    //! May have to set pinCount manually if build fails for arduino
    static constexpr size_t pin_count = sizeof(pins) / sizeof(PinEntry);
    using ConstIterator = const PinEntry*;
    static constexpr ConstIterator begin() noexcept { return pins; }
    static constexpr ConstIterator end() noexcept { return pins + pin_count; }
};

#endif