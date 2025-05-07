#include <unity.h>
#include "eeprom_device_id.h"
#include "eeprom_24lc64.h"
#include "mock_i2c.h"
#include <thread>
#include <chrono>

// Empty setUp and tearDown functions for PlatformIO 6+
void setUp(void) {}
void tearDown(void) {}

// Helper function to wait for write cycle (5ms + margin)
void wait_write_cycle() {
    std::this_thread::sleep_for(std::chrono::milliseconds(6));
}

void test_eeprom_device_id_read_existing() {
    MockI2C mock_i2c;
    Eeprom24LC64 eeprom(mock_i2c, 0x50);

    // Write a valid DeviceId to EEPROM
    uint8_t existing_uuid[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
                              0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
    DeviceId existing_id(existing_uuid);
    EepromError result = eeprom.write(0, reinterpret_cast<const uint8_t*>(&existing_id), DeviceId::SIZE);
    TEST_ASSERT_EQUAL(EepromError::SUCCESS, result);

    // Wait for write cycle to complete
    wait_write_cycle();

    // Construct EepromDeviceId, should read existing ID
    uint8_t new_uuid[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                          0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    EepromDeviceId device_id(new_uuid, eeprom, 0);

    // Verify it used the existing ID
    const DeviceId& id = device_id.get_device_id();
    TEST_ASSERT_TRUE(id.is_valid());
    TEST_ASSERT_EQUAL_MEMORY(existing_uuid, id.unique_id, 16);
    TEST_ASSERT_EQUAL_MEMORY("DID:", id.prefix, 4);
    TEST_ASSERT_EQUAL_UINT8(0x01, id.version);
}

void test_eeprom_device_id_create_new() {
    MockI2C mock_i2c;
    Eeprom24LC64 eeprom(mock_i2c, 0x50);

    // Write an invalid DeviceId (corrupted checksum)
    uint8_t invalid_uuid[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
                              0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
    DeviceId invalid_id(invalid_uuid);
    invalid_id.checksum ^= 0xFFFF; // Corrupt checksum
    EepromError result = eeprom.write(0, reinterpret_cast<const uint8_t*>(&invalid_id), DeviceId::SIZE);
    TEST_ASSERT_EQUAL(EepromError::SUCCESS, result);

    // Wait for write cycle to complete
    wait_write_cycle();

    // Construct EepromDeviceId, should use new UUID
    uint8_t new_uuid[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11,
                          0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99};
    EepromDeviceId device_id(new_uuid, eeprom, 0);

    // Verify it used the new UUID
    const DeviceId& id = device_id.get_device_id();
    TEST_ASSERT_TRUE(id.is_valid());
    TEST_ASSERT_EQUAL_MEMORY(new_uuid, id.unique_id, 16);
    TEST_ASSERT_EQUAL_MEMORY("DID:", id.prefix, 4);
    TEST_ASSERT_EQUAL_UINT8(0x01, id.version);

    // Destroy device_id, should write new ID to EEPROM
    device_id.~EepromDeviceId();

    // Wait for write cycle to complete
    wait_write_cycle();

    // Read back to verify
    DeviceId read_id;
    result = eeprom.read(0, reinterpret_cast<uint8_t*>(&read_id), DeviceId::SIZE);
    TEST_ASSERT_EQUAL(EepromError::SUCCESS, result);
    TEST_ASSERT_TRUE(read_id.is_valid());
    TEST_ASSERT_EQUAL_MEMORY(new_uuid, read_id.unique_id, 16);
}

void test_eeprom_device_id_no_write_if_valid() {
    MockI2C mock_i2c;
    Eeprom24LC64 eeprom(mock_i2c, 0x50);

    // Write a valid DeviceId
    uint8_t existing_uuid[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
                              0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
    DeviceId existing_id(existing_uuid);
    EepromError result = eeprom.write(0, reinterpret_cast<const uint8_t*>(&existing_id), DeviceId::SIZE);
    TEST_ASSERT_EQUAL(EepromError::SUCCESS, result);

    // Wait for write cycle to complete
    wait_write_cycle();

    // Construct and destroy EepromDeviceId with different UUID
    uint8_t new_uuid[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                          0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    {
        EepromDeviceId device_id(new_uuid, eeprom, 0);
        // Verify it used existing ID
        const DeviceId& id = device_id.get_device_id();
        TEST_ASSERT_EQUAL_MEMORY(existing_uuid, id.unique_id, 16);
    } // Destructor runs here

    // Read back to verify existing ID was not overwritten
    DeviceId read_id;
    result = eeprom.read(0, reinterpret_cast<uint8_t*>(&read_id), DeviceId::SIZE);
    TEST_ASSERT_EQUAL(EepromError::SUCCESS, result);
    TEST_ASSERT_TRUE(read_id.is_valid());
    TEST_ASSERT_EQUAL_MEMORY(existing_uuid, read_id.unique_id, 16);
}

void test_eeprom_device_id_invalid_address() {
    MockI2C mock_i2c;
    Eeprom24LC64 eeprom(mock_i2c, 0x50);

    uint8_t uuid[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11,
                      0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99};

    // Construct with invalid address (exceeds memory size)
    EepromDeviceId device_id(uuid, eeprom, 8191);
    const DeviceId& id = device_id.get_device_id();
    TEST_ASSERT_TRUE(id.is_valid());
    TEST_ASSERT_EQUAL_MEMORY(uuid, id.unique_id, 16);

    // Destroy, should not write due to invalid address
    device_id.~EepromDeviceId();

    // Verify no write occurred (read should fail)
    DeviceId read_id;
    EepromError result = eeprom.read(8191, reinterpret_cast<uint8_t*>(&read_id), DeviceId::SIZE);
    TEST_ASSERT_EQUAL(EepromError::INVALID_ADDRESS, result);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_eeprom_device_id_read_existing);
    RUN_TEST(test_eeprom_device_id_create_new);
    RUN_TEST(test_eeprom_device_id_no_write_if_valid);
    RUN_TEST(test_eeprom_device_id_invalid_address);
    return UNITY_END();
}