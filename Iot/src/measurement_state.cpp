/**
 * @file measurement_state.cpp
 * @brief Handles sensor measurements over time for Arduino UNO R4
 * 
 */
#include "measurement_state.h"
#include <cmath>  // For NAN
#include <cstring>  // for memcpy
#include <cstdint>  // for size_t

MeasurementState::MeasurementState(uint8_t pir_pin, unsigned long hold_duration, uint8_t temp_sensor_pin)
    : m_pir_pin { pir_pin }, m_hold_duration { hold_duration }, m_temp_sensor { temp_sensor_pin }
    {
        if (temp_sensor_pin == 0) 
        {
            m_temp_sensor_connected = false;
        } else 
        {
            m_temp_sensor_connected = true;
        }
    }

unsigned long MeasurementState::get_current_time(){
    return millis();
}

bool MeasurementState::get_new_temperature_offset_from_serial(float& offset)
{
String serial_message = Serial.readStringUntil('\n');    
    if (serial_message.length()>0) {
      serial_message.trim();  // remove whitespace at end of String
      int sign;
      // check for valid commands
      if (serial_message.substring(0, 4) == "TEMP") {
        if (serial_message.substring(4,5) == "0") 
        {
          sign = 0;
        } else if (serial_message.substring(4,5) == "+")
        {
          sign = 1;
        }
        else if (serial_message.substring(4,5) == "-")
        {
          sign = -1;
        }
        else 
        {
          return false;
        }
        float value = serial_message.substring(5).toFloat();
        if (sign == 0 || value != 0)
        {
          offset = value * sign;
          return true;
        }
      }
      return false;
    }
}

bool MeasurementState::writeFloatToEEPROM(uint16_t eeprom_addr, size_t buffer_index, float value)
{
    if (!m_eeprom) {
        return false;
    }
    std::array<uint8_t, 4> data;
	std::memcpy(data.data(), &value, sizeof(float));
	
	const std::uint16_t byte_address = eeprom_addr + (buffer_index * sizeof(float));
    
    if (m_eeprom->writeBlock(byte_address, data.data(), sizeof(float)) == 0) {
        return true;
    }
	return false;
}
bool MeasurementState::readFloatFromEEPROM(uint16_t eeprom_addr, size_t buffer_index, float* destination)
{
    if (!m_eeprom) {
        return false;
    }

    std::array<std::uint8_t, 4> data;
    
    const std::uint16_t byte_address = eeprom_addr + (buffer_index * sizeof(float));
    if (m_eeprom->readBlock(byte_address, data.data(), sizeof(float)) != sizeof(float)) {
        return false;
    }
    std::memcpy(destination, data.data(), sizeof(float));
    return true;

}


void MeasurementState::begin(I2C_eeprom* eeprom){
    // initialize internal EEPROM
    m_eeprom = eeprom;

    // initialize PIR sensor
    pinMode(m_pir_pin, INPUT);

    // initialize SGP30
    if(! m_sgp.begin()) {
        Serial.println(F("SGP30 sensor not initialized"));
    }
    else {
        Serial.println(F("SGP30 sensor initialized"));
        if (m_sgp.getIAQBaseline(&m_iaq_baseline_eco2, &m_iaq_baseline_tvoc)) {
            Serial.println(F("SGP30 sensor baseline values set"));
            m_sgp_initialized = true;
        }
        else {
            Serial.println(F("SGP30 sensor baseline values not set"));
        }
    }
    // Initialize DTH11 Temperature Sensor.
    // No return value is given from begin()
    if (m_temp_sensor_connected) {
        m_temp_sensor.begin();
        #ifdef MAIN_STATE_DEBUG
        Serial.println("DHT11 begin function called");
        #endif
    }
}


bool MeasurementState::update_temperature_offset_from_eeprom(uint16_t address)
    {
        float new_offset;
        size_t offset_index { 0 };
        if (!readFloatFromEEPROM(address, offset_index, &new_offset))
        {
            return false;
        }
        Serial.print("Temperature offset from EEPROM: ");
        Serial.println(new_offset);
        if(set_temperature_offset(new_offset))
        {
          Serial.print("Setting saved temperature calibration: ");
          Serial.println(new_offset);
          return true;
        }      
        return false;
    }

bool MeasurementState::update_temperature_offset_from_serial(uint16_t eeprom_address)
{
  // in case caller forgot, check if a message is available
  if (Serial.available())
  {
    float new_offset{}; 
    if (get_new_temperature_offset_from_serial(new_offset)) 
    {
      if(set_temperature_offset(new_offset))
      {
          Serial.print("Setting new temperature calibration: ");
          Serial.println(new_offset);
          
          size_t buffer_index { 0 };
          if (writeFloatToEEPROM(eeprom_address, buffer_index, new_offset))
          {
              return true;
          }
      }  
    }
  }
  return false;
}

bool MeasurementState::set_temperature_offset(float new_temperature_offset)
{
    bool set_offset_successful = m_temp_sensor.set_temperature_offset(new_temperature_offset);
if (set_offset_successful){
    size_t offset_index { 0 };
    }

    return set_offset_successful;
}

float MeasurementState::get_temperature_offset()
{
    return m_temp_sensor.get_temperature_offset();
}

void MeasurementState::update_pir()
{
    // update room status
    PinStatus currentPinReading = digitalRead(m_pir_pin);
    if (currentPinReading) {
        m_last_activation_time = get_current_time();
      }
}
void MeasurementState::update_all(){
    // Read PIR Sensor
    update_pir();
    // read Air Quality Sensor
    if (m_sgp_initialized) {
    read_air_quality();
    }
    // Read Temperature Sensor
    if (m_temp_sensor_connected) {
        m_temperature = m_temp_sensor.get_temperature();
        m_humidity = m_temp_sensor.get_humidity();
    }
}

String MeasurementState::room_has_activity(){
    if (get_current_time() - m_last_activation_time <= m_hold_duration) {
        return "true";
      } else {
        return "false";
      }    
}

bool MeasurementState::room_has_activity_bool() {
    return get_current_time() - m_last_activation_time <= m_hold_duration;
}

void MeasurementState::read_air_quality()
{
    m_sgp.setHumidity(0);
    // try to read new data from Air Quality Sensor
    if (!m_sgp.IAQmeasure()) {
        Serial.println(F("Air Quality Sensor reading failed"));
    }
    // if successful update cached Air Quality value
    else {
        // convert ppm reading to Air Quality in percent using exponential scaling
        // 400 ppm gives 100% air quality, 1000 ppm 55% and 2000 ppm 22%
        // It's possible to improve accuracy of conversion by incorporating humidity readings from DHT11
        float air_quality = 100 * exp(-0.001 * (m_sgp.eCO2 - 400));
        m_air_quality = constrain(air_quality, 0, 100);
    }
}

String MeasurementState::get_air_quality()
{
    if (!m_sgp_initialized) {
        return F("Sensor error");
    }
    return String(m_air_quality);
}

float MeasurementState::get_air_quality_float()
{
    if (!m_sgp_initialized) {
        return NAN;  // Return NAN to indicate error
    }
    return m_air_quality;
}
    
String MeasurementState::get_temperature()
{
    if (!m_temp_sensor_connected) {
        return F("Sensor error");
    }
    return String(m_temperature);
}

float MeasurementState::get_temperature_float()
{
    if (!m_temp_sensor_connected) {
        return NAN;  // Return NAN to indicate error
    }
    return m_temperature;
}
