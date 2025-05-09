
//! @file uuid.h
//! @author Jennifer Gott
//! @date 2025-05-09
//! @brief 128-bit UUID
//! @details This file contains the UUID type and the constructor for it, using a MAC address as a source.
//! We generate a v4 variant 1 UUID, which is a semi-random UUID, filling the non-MAC address bytes with random data.
//! We use the entropy source and the clock source to seed the random number generator.


#ifndef INTERFACES_UUID_H
#define INTERFACES_UUID_H

#include <array> //!< std::array
#include <cstring> //!< memcpy
#include "random_source.h" //!< Random number generator
#include "clock_source.h" //!< Clock source, for extra entropy
#include "pin_io.h" //!< Pin IO for analog read
#include "pin_config.h" //!< Pin name for the entropy source pin


//! @brief 128-bit UUID
struct Uuid {
    std::array<uint8_t, 16> bytes;

    //! @brief Default constructor
    //! @details This constructor is used to initialize the UUID to all zeros.
    Uuid() : bytes() {}

    //! @brief Constructor
    //! @param mac_address The MAC address to use for the UUID
    //! @param random_source The random number generator to use for the UUID
    //! @param clock_source The clock source to use for entropy
    //! @param pin_io The pin IO to use for entropy
    //! @note Expected call signature: const Uuid new_uuid = Uuid(network_interface.get_mac_address(), random_source, time, pin_io);
    Uuid(const std::array<uint8_t, 6>& mac_address, RandomSource& random_source, Clock& clock_source, PinIO& pin_io) {

        //! @note We use the entropy source and the clock source to seed the random number generator.
        //! We subtract the clock source from the entropy source to get a more random seed, since analogRead has limited resolution.
        //! Strictly speaking we should check that this is a non-zero number since Arduino randomSeed noop on zero input.
        //! However, this is unlikely to happen and the entropy source is not a hardware RNG.
        int32_t seed = pin_io.analogRead(PinConfig::Pin::ENTROPY_SOURCE) - clock_source.millis();
        random_source.randomSeed(seed ? seed : 0x80); //! I just picked a constant we use already


        //! Fill the first 10 bytes with random data
        for (size_t i = 0; i < 10; i++) {
            bytes[i] = static_cast<uint8_t>(random_source.random(256));
        }
        //! Version and variant bits
        bytes[7] = (static_cast<uint8_t>(random_source.random(256)) & 0x0F) | 0x40; // Version 4
        bytes[8] = (static_cast<uint8_t>(random_source.random(256)) & 0x3F) | 0x80; // Variant 1

        //! Fill the next 6 bytes with the MAC address
        std::memcpy(bytes.data() + 10, mac_address.data(), 6);
    }
};

#endif  // INTERFACES_UUID_H