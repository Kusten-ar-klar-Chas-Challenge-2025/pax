#ifndef DEVICE_H
#define DEVICE_H
#include <cstdint>

/**
 * @brief Namespace for grouping constats
 */
namespace Device {

    //! Represenst a uniquie MAC-ADDRESS
    constexpr uint8_t ETHERNET_MAC[] = { 0x02, 0x00, 0x00, 0x00, 0x00, 0x01 };

    //! Represent a IP address if others fail
    constexpr uint8_t FALLBACK_IP[] = { 192, 168, 1, 177 }
};

#endif