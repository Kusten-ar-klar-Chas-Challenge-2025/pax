

#ifndef TEST_COMMON_MOCK_DEVICE_ID_H
#define TEST_COMMON_MOCK_DEVICE_ID_H

#ifdef UNIT_TEST

#include "interfaces/device_id.h"

#include <string>

class MockDeviceId : public DeviceId {
    public:
        MockDeviceId(std::string id) : id(id) {}
        const char* get_id() override { return id.c_str(); }
        std::string_view get_id_as_string_view() override { return id; }
        void set_id(std::string id) { this->id = id; }
    private:
        std::string id;
};

#endif // UNIT_TEST

#endif // TEST_COMMON_MOCK_DEVICE_ID_H