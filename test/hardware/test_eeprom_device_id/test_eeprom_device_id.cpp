//! @file test_eeprom_device_id.cpp
//! @brief Test cases for the EepromDeviceId class
//! @author Jennifer Gott (simbachu@gmail.com)
//! @date 2025-05-08


#include <unity.h>
#include "eeprom_device_id.h"
#include "eeprom_24lc64.h"
#include "uuid.h"
#include "clock_source.h"
#include "random_source.h"
#include "pin_io.h"
#include "mock_i2c.h"
#include "mock_random_source.h"
#include "mock_clock_source.h"
#include "mock_io.h"
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
    Uuid existing_uuid;
    existing_uuid.bytes = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
                              0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
    DeviceId existing_id(existing_uuid);
    EepromError result = eeprom.write(0, reinterpret_cast<const uint8_t*>(&existing_id), DeviceId::SIZE);
    TEST_ASSERT_EQUAL(EepromError::SUCCESS, result);

    // Wait for write cycle to complete
    wait_write_cycle();

    // Construct EepromDeviceId, should read existing ID
    Uuid new_uuid;
    new_uuid.bytes = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                          0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    EepromDeviceId device_id(new_uuid, eeprom, 0);

    // Verify it used the existing ID
    const DeviceId& id = device_id.get_device_id();
    TEST_ASSERT_TRUE(id.is_valid());
    TEST_ASSERT_EQUAL_MEMORY(existing_uuid.bytes.data(), id.unique_id, 16);
    TEST_ASSERT_EQUAL_MEMORY("DID:", id.prefix, 4);
    TEST_ASSERT_EQUAL_UINT8(0x01, id.version);
}

void test_eeprom_device_id_create_new() {
    MockI2C mock_i2c;
    Eeprom24LC64 eeprom(mock_i2c, 0x50);

    // Write an invalid DeviceId (corrupted checksum)
    Uuid invalid_uuid;
    invalid_uuid.bytes = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
                              0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
    DeviceId invalid_id(invalid_uuid);
    invalid_id.checksum ^= 0xFFFF; // Corrupt checksum
    EepromError result = eeprom.write(0, reinterpret_cast<const uint8_t*>(&invalid_id), DeviceId::SIZE);
    TEST_ASSERT_EQUAL(EepromError::SUCCESS, result);

    // Wait for write cycle to complete
    wait_write_cycle();

    // Construct EepromDeviceId, should use new UUID
    std::array<uint8_t, 6> mac_address = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
    Uuid new_uuid;
    new_uuid.bytes = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11,
                      0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99};
    EepromDeviceId device_id(new_uuid, eeprom, 0);

    // Verify it used the new UUID
    const DeviceId& id = device_id.get_device_id();
    TEST_ASSERT_TRUE(id.is_valid());
    TEST_ASSERT_EQUAL_MEMORY(new_uuid.bytes.data(), id.unique_id, 16);
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
    TEST_ASSERT_EQUAL_MEMORY(new_uuid.bytes.data(), read_id.unique_id, 16);
}

void test_eeprom_device_id_no_write_if_valid() {
    MockI2C mock_i2c;
    Eeprom24LC64 eeprom(mock_i2c, 0x50);

    // Write a valid DeviceId
    Uuid existing_uuid;
    existing_uuid.bytes = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
                              0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
    DeviceId existing_id(existing_uuid);
    EepromError result = eeprom.write(0, reinterpret_cast<const uint8_t*>(&existing_id), DeviceId::SIZE);
    TEST_ASSERT_EQUAL(EepromError::SUCCESS, result);

    // Wait for write cycle to complete
    wait_write_cycle();

    // Construct and destroy EepromDeviceId with different UUID
    Uuid new_uuid;
    new_uuid.bytes = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                          0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    {
        EepromDeviceId device_id(new_uuid, eeprom, 0);
        // Verify it used existing ID
        const DeviceId& id = device_id.get_device_id();
        TEST_ASSERT_EQUAL_MEMORY(existing_uuid.bytes.data(), id.unique_id, 16);
    } // Destructor runs here

    // Read back to verify existing ID was not overwritten
    EepromDeviceId read_id(new_uuid, eeprom, 0);
    DeviceId temp_id;  // Create a non-const copy
    result = eeprom.read(0, reinterpret_cast<uint8_t*>(&temp_id), DeviceId::SIZE);
    TEST_ASSERT_EQUAL(EepromError::SUCCESS, result);
    TEST_ASSERT_TRUE(temp_id.is_valid());
    TEST_ASSERT_EQUAL_MEMORY(existing_uuid.bytes.data(), temp_id.unique_id, 16);
}

void test_eeprom_device_id_invalid_address() {
    MockI2C mock_i2c;
    Eeprom24LC64 eeprom(mock_i2c, 0x50);

    Uuid uuid;
    uuid.bytes = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11,
                      0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99};

    // Construct with invalid address (exceeds memory size)
    EepromDeviceId device_id(uuid, eeprom, 8191);
    const DeviceId& id = device_id.get_device_id();
    TEST_ASSERT_TRUE(id.is_valid());
    TEST_ASSERT_EQUAL_MEMORY(uuid.bytes.data(), id.unique_id, 16);

    // Destroy, should not write due to invalid address
    device_id.~EepromDeviceId();

    // Verify no write occurred (read should fail)
    DeviceId read_id;
    EepromError result = eeprom.read(8191, reinterpret_cast<uint8_t*>(&read_id), DeviceId::SIZE);
    TEST_ASSERT_EQUAL(EepromError::INVALID_ADDRESS, result);
}

//! @test Test that the UUID is created correctly
void test_eeprom_device_id_valid_uuid(){
    MockI2C mock_i2c;
    Eeprom24LC64 eeprom(mock_i2c, 0x50);
    MockRandom random_source;
    MockClock clock_source;
    MockPinIO pin_io;

    //! Set the random source to a known value
    //! Ten calls to random() will return the same value since we give a vector that will be used regardless of the seed
    random_source.set_fake_random_numbers({0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11, 0x22, 0x33});

    std::array<uint8_t, 6> mac_address = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
    Uuid uuid(mac_address, random_source, clock_source, pin_io);

    EepromDeviceId device_id(uuid, eeprom, 0);
    const DeviceId& id = device_id.get_device_id();
    
    // Verify basic validity and UUID
    TEST_ASSERT_TRUE(id.is_valid());
    TEST_ASSERT_EQUAL_MEMORY(uuid.bytes.data(), id.unique_id, 16);
    
    // Verify prefix is "DID:"
    TEST_ASSERT_EQUAL_MEMORY("DID:", id.prefix, 4);
    
    // Verify version is 0x01
    TEST_ASSERT_EQUAL_UINT8(0x01, id.version);
    
    // Verify reserved bytes are all zeros
    std::array<uint8_t, sizeof(id.reserved)> zeros = {0};
    TEST_ASSERT_EQUAL_MEMORY(zeros.data(), id.reserved, sizeof(id.reserved));
    
    // Verify checksum is correct by recalculating it
    uint16_t calculated_checksum = id.compute_crc16();
    TEST_ASSERT_EQUAL_UINT16(calculated_checksum, id.checksum);
}




int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_eeprom_device_id_read_existing);
    RUN_TEST(test_eeprom_device_id_create_new);
    RUN_TEST(test_eeprom_device_id_no_write_if_valid);
    RUN_TEST(test_eeprom_device_id_invalid_address);
    RUN_TEST(test_eeprom_device_id_valid_uuid);
    return UNITY_END();
}