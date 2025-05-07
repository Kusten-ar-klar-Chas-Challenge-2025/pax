#include <unity.h>
#include "mock_eeprom.h"
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

//! @test Test the write and read functions
//! @author Jennifer Gott
//! @date 2025-05-07
void test_write_read_single_byte() {
    //! Arrange
    MockEeprom eeprom;
    uint8_t source = 0xAA;
    uint8_t destination = 0;

    //! Act
    EepromError result = eeprom.write(0, &source, 1);
    TEST_ASSERT_EQUAL(EepromError::SUCCESS, result);
    result = eeprom.read(0, &destination, 1);
    TEST_ASSERT_EQUAL(EepromError::SUCCESS, result);

    //! Assert
    TEST_ASSERT_EQUAL_UINT8(0xAA, destination);
}

//! @test Test the write and read functions
//! @author Jennifer Gott
//! @date 2025-05-07
void test_write_read_multi_byte() {
    //! Arrange
    MockEeprom eeprom;

    uint8_t source[] = {0xAA, 0xBB, 0xCC};
    uint8_t destination[3] = {0};

    //! Act
    //! Write 3 bytes
    EepromError result = eeprom.write(0, source, 3);
    TEST_ASSERT_EQUAL(EepromError::SUCCESS, result);

    //! Read second byte from address+1
    uint8_t second_byte = 0;
    result = eeprom.read(1, &second_byte, 1);
    TEST_ASSERT_EQUAL(EepromError::SUCCESS, result);
    TEST_ASSERT_EQUAL_UINT8(0xBB, second_byte);

    //! Read all 3 bytes
    result = eeprom.read(0, destination, 3);
    TEST_ASSERT_EQUAL(EepromError::SUCCESS, result);

    //! Assert
    TEST_ASSERT_EQUAL_UINT8_ARRAY(source, destination, 3);
}

//! @test Test the write from a string view function
//! @author Jennifer Gott
//! @date 2025-05-07
void test_write_string_view() {
    //! Arrange
    MockEeprom eeprom;
    std::string_view source = "TEST";
    uint8_t destination[4] = {0};

    //! Act
    //! Write string view
    EepromError result = eeprom.write_string_view(0, source);
    TEST_ASSERT_EQUAL(EepromError::SUCCESS, result);

    //! Read back
    result = eeprom.read(0, destination, 4);
    TEST_ASSERT_EQUAL(EepromError::SUCCESS, result);
    TEST_ASSERT_EQUAL_UINT8_ARRAY("TEST", destination, 4);

    //! Verify second byte at address+1
    uint8_t second_byte = 0;
    result = eeprom.read(1, &second_byte, 1);
    TEST_ASSERT_EQUAL(EepromError::SUCCESS, result);
    //! Assert
    TEST_ASSERT_EQUAL_UINT8('E', second_byte);
}

//! @test Test writing past page boundary
//! @author Jennifer Gott
//! @date 2025-05-07
void test_page_boundary_violation() {
    //! Arrange
    MockEeprom eeprom;
    uint8_t source[33]; // 33 bytes, exceeds page size
    std::fill_n(source, 33, 0xFF);

    //! Act
    //! Write crossing page boundary (address 31 + 33 bytes)
    EepromError result = eeprom.write(31, source, 33);
    TEST_ASSERT_EQUAL(EepromError::INVALID_ADDRESS, result);

    //! Write within page boundary (address 32, start of next page)
    result = eeprom.write(32, source, 32); // Fits exactly in page
    TEST_ASSERT_EQUAL(EepromError::SUCCESS, result);
}

//! @test Test the write cycle protection
//! @author Jennifer Gott
//! @date 2025-05-07
void test_write_cycle_protection() {
    //! Arrange
    MockEeprom eeprom;
    uint8_t source[] = {0xAA, 0xBB};

    //! Act
    //! First write
    EepromError result = eeprom.write(0, source, 2);
    TEST_ASSERT_EQUAL(EepromError::SUCCESS, result);

    //! Immediate second write (within 5ms)
    result = eeprom.write(2, source, 2);
    TEST_ASSERT_EQUAL(EepromError::WRITE_PROTECTED, result);

    //! Wait for write cycle to complete
    wait_write_cycle();

    //! Retry write
    result = eeprom.write(2, source, 2);
    //! Assert  
    TEST_ASSERT_EQUAL(EepromError::SUCCESS, result);
}

//! @test Test writing beyond memory size
//! @author Jennifer Gott
//! @date 2025-05-07
void test_invalid_address() {
    //! Arrange
    MockEeprom eeprom;
    uint8_t source[] = {0xAA};
    uint8_t destination = 0;

    //! Act
    //! Write beyond memory size
    EepromError result = eeprom.write(8192, source, 1);
    TEST_ASSERT_EQUAL(EepromError::INVALID_ADDRESS, result);

    //! Read beyond memory size
    result = eeprom.read(8192, &destination, 1);
    TEST_ASSERT_EQUAL(EepromError::INVALID_ADDRESS, result);

    //! Write spanning beyond memory size
    result = eeprom.write(8191, source, 2);
    //! Assert
    TEST_ASSERT_EQUAL(EepromError::INVALID_ADDRESS, result);
}

//! @test Test writing with null buffer
//! @author Jennifer Gott
//! @date 2025-05-07
void test_null_buffer() {
    //! Arrange
    MockEeprom eeprom;
    uint8_t destination = 0;

    //! Act
    //! Write with null source
    EepromError result = eeprom.write(0, nullptr, 1);
    TEST_ASSERT_EQUAL(EepromError::BUFFER_NULLPTR, result);

    //! Read with null destination
    result = eeprom.read(0, nullptr, 1);
    //! Assert
    TEST_ASSERT_EQUAL(EepromError::BUFFER_NULLPTR, result);
}



//! @test Test the eeprom interface
//! @author Jennifer Gott
//! @date 2025-05-07
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_write_read_single_byte);
    RUN_TEST(test_write_read_multi_byte);
    RUN_TEST(test_write_string_view);
    RUN_TEST(test_page_boundary_violation);
    RUN_TEST(test_write_cycle_protection);
    RUN_TEST(test_invalid_address);
    RUN_TEST(test_null_buffer);
    return UNITY_END();
}