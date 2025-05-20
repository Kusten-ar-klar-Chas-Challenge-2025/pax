#include "measurement_state.h"


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

void MeasurementState::begin(){
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
    }
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



bool MeasurementState::room_has_activity(){
    if (get_current_time() - m_last_activation_time <= m_hold_duration) {
        return true;
      } else {
        return false;
      }    
}

String MeasurementState::room_is_available()
{
    if (room_has_activity()) {
        return "false";
    } else
    return "true";
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
        m_air_quality = m_sgp.eCO2;
    }
}

 String MeasurementState::get_air_quality()
 {
    if (!m_sgp_initialized) {
        return F("Sensor error");
    }
    return String(m_air_quality);
 }
    
 String MeasurementState::get_temperature()
 {
    if (!m_temp_sensor_connected) {
        return F("Sensor error");
    }
    return String(m_temperature);
 }