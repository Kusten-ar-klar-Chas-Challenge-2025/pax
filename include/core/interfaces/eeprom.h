#ifndef INTERFACES_EEPROM_H
#define INTERFACES_EEPROM_H

#include <cstdint>
#include <string_view>

//! @brief Enum for EEPROM error codes
//! @note First four values are defined by return value of Wire.endTransmission() from the Wire library
enum class EepromError {
    SUCCESS = 0,           //!< Transmission completed successfully, defined by return value of Wire.endTransmission()
    DATA_TOO_LONG = 1,     //!< Data too long to fit in transmit buffer, defined by return value of Wire.endTransmission()
    NACK_ADDRESS = 2,      //!< Received NACK on transmit of address, defined by return value of Wire.endTransmission()
    NACK_DATA = 3,         //!< Received NACK on transmit of data, defined by return value of Wire.endTransmission()
    OTHER_ERROR = 4,       //!< Other error, defined by return value of Wire.endTransmission()
    TIMEOUT,           //!< Timeout, device not responding
    BUFFER_OVERFLOW,   //!< Buffer overflow (read requested more bytes than available)
    WRITE_PROTECTED,   //!< EEPROM is write protected or write cycle in progress
    INVALID_ADDRESS,   //!< Address out of bounds for EEPROM
    BUFFER_NULLPTR    //!< Buffer pointer is nullptr
};

//! @brief Interface for EEPROM operations
class Eeprom {
    public:
        //! @brief Write data to EEPROM
        //! @param address The address to write to
        //! @param[in] source The data to write
        //! @param length The length of the data to write
        virtual EepromError write(uint16_t address, const uint8_t* source, uint16_t length) = 0;

        //! @brief Read data from EEPROM
        //! @param address The address to read from
        //! @param[out] destination The data to read
        //! @param length The length of the data to read
        virtual EepromError read(uint16_t address, uint8_t* destination, uint16_t length) = 0;

        //! @brief Write a string view to EEPROM
        //! @param address The address to write to
        //! @param[in] source The string view to write
        virtual EepromError write(uint16_t address, std::string_view source) = 0;
};

#endif // INTERFACES_EEPROM_H