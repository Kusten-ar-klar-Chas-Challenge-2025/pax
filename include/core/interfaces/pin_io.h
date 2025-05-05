#ifndef CORE_INTERFACES_PIN_IO_H
#define CORE_INTERFACES_PIN_IO_H

#include <stdint.h>

//! @brief Base class for pin IO
//! @details Interface class to handle IO from GPIO on Arduino
//! Abstracted interface to be able to mock the IO in unit tests
class PinIO
{
    protected:
    //! @brief Default constructor, protected to allow derived class construction
    PinIO () = default;

    public:
    //! @brief Default destructor, virtual to allow destruction of the derived class
    virtual ~PinIO () = default;
    //! @brief Delete copy constructor to prevent copying
    PinIO(const PinIO &) = delete;
    //! @brief Delete assignment operator to prevent assignment
    PinIO &operator=(const PinIO &) = delete;
    //! @brief Setup the pin
    //! @note This should be called before any other functions
    virtual void setup() noexcept = 0;
    //! @brief Read the pin
    //! Nodiscard because pin reads should be checked
    [[nodiscard]] virtual bool digitalRead(uint8_t pin) noexcept = 0;
    //! @brief Write the pin
    virtual void digitalWrite(uint8_t pin, bool value) noexcept = 0;
    //! @brief Read analog value
    //! Nodiscard because pin reads should be checked
    [[nodiscard]] virtual uint16_t analogRead(uint8_t pin) noexcept = 0;
    //! @brief Write analog value
    virtual void analogWrite(uint8_t pin, uint16_t value) noexcept = 0;  
    //! @brief Set the mode of a pin
    virtual void pinMode(uint8_t pin, uint8_t mode) noexcept = 0;
};

#endif

