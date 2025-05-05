#ifndef DEVICE_H
#define DEVICE_H
#include <cstdint>

namespace Device {

    constexpr uint8_t ETHERNET_MAC[] = { 0x02, 0x00, 0x00, 0x00, 0x00, 0x01 };
    constexpr uint8_t FALLBACK_IP[] = { 192, 168, 1, 177 };

};

#endif