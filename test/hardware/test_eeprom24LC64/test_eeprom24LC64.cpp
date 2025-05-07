#include <unity.h>
#include "eeprom_24LC64.h"
#include "mock_i2c.h"
#include <thread>
#include <chrono>

void setUp(void) {
    // This is run before each test
}

void tearDown(void) {
    // This is run after each test
}

// Helper function to wait for write cycle (5ms + margin)
void wait_write_cycle() {
    std::this_thread::sleep_for(std::chrono::milliseconds(6));
}

void test_write_read_single_byte() {
    // Local test fixtures
    MockI2C mock_i2c;
    Eeprom24LC64 eeprom(mock_i2c, 0x50); // I2C device address 0x50

    uint8_t source = 0xAA;
    uint8_t destination = 0;

    // Write single byte
    EepromError result = eeprom.write(0, &source, 1);
    TEST_ASSERT_EQUAL(EepromError::SUCCESS, result);

    // Read single byte
    result = eeprom.read(0, &destination, 1);
    TEST_ASSERT_EQUAL(EepromError::SUCCESS, result);
    TEST_ASSERT_EQUAL_UINT8(0xAA, destination);
}

void test_write_read_multi_byte() {
    // Local test fixtures
    MockI2C mock_i2c;
    Eeprom24LC64 eeprom(mock_i2c, 0x50); // I2C device address 0x50

    uint8_t source[] = {0xAA, 0xBB, 0xCC};
    uint8_t destination[3] = {0};

    // Write 3 bytes
    EepromError result = eeprom.write(0, source, 3);
    TEST_ASSERT_EQUAL(EepromError::SUCCESS, result);

    // Read second byte from address+1
    uint8_t second_byte = 0;
    result = eeprom.read(1, &second_byte, 1);
    TEST_ASSERT_EQUAL(EepromError::SUCCESS, result);
    TEST_ASSERT_EQUAL_UINT8(0xBB, second_byte);

    // Read all 3 bytes
    result = eeprom.read(0, destination, 3);
    TEST_ASSERT_EQUAL(EepromError::SUCCESS, result);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(source, destination, 3);
}

void test_write_string_view() {
    // Local test fixtures
    MockI2C mock_i2c;
    Eeprom24LC64 eeprom(mock_i2c, 0x50); // I2C device address 0x50

    std::string_view source = "TEST";
    uint8_t destination[4] = {0};

    // Write string view
    EepromError result = eeprom.write_string_view(0, source);
    TEST_ASSERT_EQUAL(EepromError::SUCCESS, result);

    // Read back
    result = eeprom.read(0, destination, 4);
    TEST_ASSERT_EQUAL(EepromError::SUCCESS, result);
    TEST_ASSERT_EQUAL_UINT8_ARRAY("TEST", destination, 4);

    // Verify second byte at address+1
    uint8_t second_byte = 0;
    result = eeprom.read(1, &second_byte, 1);
    TEST_ASSERT_EQUAL(EepromError::SUCCESS, result);
    TEST_ASSERT_EQUAL_UINT8('E', second_byte);
}

void test_page_boundary_violation() {
    // Local test fixtures
    MockI2C mock_i2c;
    Eeprom24LC64 eeprom(mock_i2c, 0x50); // I2C device address 0x50

    uint8_t source[33]; // 33 bytes, exceeds page size
    std::fill_n(source, 33, 0xFF);

    // Write crossing page boundary (address 31 + 10 bytes)
    EepromError result = eeprom.write(31, source, 10);
    TEST_ASSERT_EQUAL(EepromError::INVALID_ADDRESS, result);

    // Write exceeding page size
    result = eeprom.write(0, source, 33);
    TEST_ASSERT_EQUAL(EepromError::DATA_TOO_LONG, result);

    // Write within page boundary (address 32, start of next page)
    result = eeprom.write(32, source, 32); // Fits exactly in page
    TEST_ASSERT_EQUAL(EepromError::SUCCESS, result);
}

void test_write_cycle_protection() {
    // Local test fixtures
    MockI2C mock_i2c;
    Eeprom24LC64 eeprom(mock_i2c, 0x50); // I2C device address 0x50

    uint8_t source[] = {0xAA, 0xBB};

    // First write
    EepromError result = eeprom.write(0, source, 2);
    TEST_ASSERT_EQUAL(EepromError::SUCCESS, result);

    // Immediate second write (within 5ms)
    result = eeprom.write(2, source, 2);
    TEST_ASSERT_EQUAL(EepromError::WRITE_PROTECTED, result);

    // Wait for write cycle to complete
    wait_write_cycle();

    // Retry write
    result = eeprom.write(2, source, 2);
    TEST_ASSERT_EQUAL(EepromError::SUCCESS, result);
}

void test_invalid_address() {
    // Local test fixtures
    MockI2C mock_i2c;
    Eeprom24LC64 eeprom(mock_i2c, 0x50); // I2C device address 0x50

    uint8_t source[] = {0xAA};
    uint8_t destination = 0;

    // Write beyond memory size
    EepromError result = eeprom.write(8192, source, 1);
    TEST_ASSERT_EQUAL(EepromError::INVALID_ADDRESS, result);

    // Write spanning beyond memory size
    result = eeprom.write(8191, source, 2);
    TEST_ASSERT_EQUAL(EepromError::INVALID_ADDRESS, result);

    // Read beyond memory size
    result = eeprom.read(8192, &destination, 1);
    TEST_ASSERT_EQUAL(EepromError::INVALID_ADDRESS, result);
}

void test_null_buffer() {
    // Local test fixtures
    MockI2C mock_i2c;
    Eeprom24LC64 eeprom(mock_i2c, 0x50); // I2C device address 0x50

    uint8_t destination = 0;

    // Write with null source
    EepromError result = eeprom.write(0, nullptr, 1);
    TEST_ASSERT_EQUAL(EepromError::BUFFER_NULLPTR, result);

    // Read with null destination
    result = eeprom.read(0, nullptr, 1);
    TEST_ASSERT_EQUAL(EepromError::BUFFER_NULLPTR, result);

    // Write string view with null data
    std::string_view null_source(nullptr, 0);
    result = eeprom.write_string_view(0, null_source);
    TEST_ASSERT_EQUAL(EepromError::BUFFER_NULLPTR, result);
}

void test_big_endian_address() {
    // Local test fixtures
    MockI2C mock_i2c;
    Eeprom24LC64 eeprom(mock_i2c, 0x50); // I2C device address 0x50

    uint8_t source[] = {0xAA, 0xBB};
    uint8_t destination = 0;

    // Write to a non-zero address (e.g., 0x1234)
    EepromError result = eeprom.write(0x1234, source, 2);
    TEST_ASSERT_EQUAL(EepromError::SUCCESS, result);

    // Read second byte from address+1 (0x1235)
    result = eeprom.read(0x1235, &destination, 1);
    TEST_ASSERT_EQUAL(EepromError::SUCCESS, result);
    TEST_ASSERT_EQUAL_UINT8(0xBB, destination);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_write_read_single_byte);
    RUN_TEST(test_write_read_multi_byte);
    RUN_TEST(test_write_string_view);
    RUN_TEST(test_page_boundary_violation);
    RUN_TEST(test_write_cycle_protection);
    RUN_TEST(test_invalid_address);
    RUN_TEST(test_null_buffer);
    RUN_TEST(test_big_endian_address);
    return UNITY_END();
}