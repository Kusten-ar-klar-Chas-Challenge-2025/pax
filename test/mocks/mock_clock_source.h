// MockClock.h
#ifndef MOCK_CLOCK_H
#define MOCK_CLOCK_H

#include "clock_source.h"
#include <ctime>
#include <chrono>
#include <string>

// Define the DateTime and TimeSpan classes for the mock
class TimeSpan {
private:
    int64_t _seconds;

public:
    TimeSpan(int64_t seconds = 0) : _seconds(seconds) {}
    TimeSpan(int32_t days, int8_t hours, int8_t minutes, int8_t seconds)
        : _seconds((int64_t)days * 86400 + 
                  (int64_t)hours * 3600 + 
                  (int64_t)minutes * 60 + 
                  seconds) {}
    
    int16_t days() const { return _seconds / 86400; }
    int8_t hours() const { return (_seconds / 3600) % 24; }
    int8_t minutes() const { return (_seconds / 60) % 60; }
    int8_t seconds() const { return _seconds % 60; }
    int64_t totalSeconds() const { return _seconds; }
    
    // Operators as needed
    TimeSpan operator+(const TimeSpan& span) const {
        return TimeSpan(_seconds + span._seconds);
    }
    TimeSpan operator-(const TimeSpan& span) const {
        return TimeSpan(_seconds - span._seconds);
    }
};

class DateTime {
private:
    uint16_t _year;
    uint8_t _month, _day, _hour, _minute, _second, _dayOfWeek;
    time_t _unixtime;
    
    void compute_unix_time() {
        std::tm timeinfo = {};
        timeinfo.tm_year = _year - 1900;
        timeinfo.tm_mon = _month - 1;
        timeinfo.tm_mday = _day;
        timeinfo.tm_hour = _hour;
        timeinfo.tm_min = _minute;
        timeinfo.tm_sec = _second;
        _unixtime = std::mktime(&timeinfo);
        
        // Update day of week
        if (_dayOfWeek == 0) {
            std::tm* ptm = std::localtime(&_unixtime);
            _dayOfWeek = ptm->tm_wday == 0 ? 7 : ptm->tm_wday;
        }
    }

public:
    DateTime(uint16_t year, uint8_t month, uint8_t day,
            uint8_t hour = 0, uint8_t minute = 0, uint8_t second = 0)
        : _year(year), _month(month), _day(day),
          _hour(hour), _minute(minute), _second(second), _dayOfWeek(0) {
        compute_unix_time();
    }

    //! Returns the total number of seconds since the start of the year
    uint32_t totalSeconds() const {
        return _month * 30 * 24 * 3600 + _day * 24 * 3600 + _hour * 3600 + _minute * 60 + _second;
    }
    
    DateTime(time_t t = 0) : _unixtime(t) {
        std::tm* ptm = std::localtime(&t);
        _year = ptm->tm_year + 1900;
        _month = ptm->tm_mon + 1;
        _day = ptm->tm_mday;
        _hour = ptm->tm_hour;
        _minute = ptm->tm_min;
        _second = ptm->tm_sec;
        _dayOfWeek = ptm->tm_wday == 0 ? 7 : ptm->tm_wday;
    }
    
    static DateTime now() {
        return DateTime(std::time(nullptr));
    }
    
    uint16_t year() const { return _year; }
    uint8_t month() const { return _month; }
    uint8_t day() const { return _day; }
    uint8_t hour() const { return _hour; }
    uint8_t minute() const { return _minute; }
    uint8_t second() const { return _second; }
    uint8_t dayOfTheWeek() const { return _dayOfWeek; }
    time_t unixtime() const { return _unixtime; }
    
    //! Operators for DateTime
    DateTime operator+(const TimeSpan& span) const 
    {
        return DateTime(_unixtime + span.totalSeconds());
    }
    
    DateTime operator-(const TimeSpan& span) const 
    {
        return DateTime(_unixtime - span.totalSeconds());
    }
    
    TimeSpan operator-(const DateTime& dt) const 
    {
        return TimeSpan(_unixtime - dt._unixtime);
    }
    
    bool operator==(const DateTime& dt) const 
    {
        return _unixtime == dt._unixtime;
    }

    bool operator!=(const DateTime& dt) const 
    {
        return _unixtime != dt._unixtime;
    }

    bool operator<(const DateTime& dt) const 
    {
        return _unixtime < dt._unixtime;
    }

    bool operator>(const DateTime& dt) const 
    {
        return _unixtime > dt._unixtime;
    }

    bool operator<=(const DateTime& dt) const 
    {
        return _unixtime <= dt._unixtime;
    }

    bool operator>=(const DateTime& dt) const 
    {
        return _unixtime >= dt._unixtime;
    }

};

class MockClock : public Clock {
private:
    DateTime set_time_value;
    bool is_running;
    int8_t trim_value;
    uint32_t set_millis_value;
    std::chrono::steady_clock::time_point start;

public:
    MockClock() 
        : set_time_value(DateTime::now()),
          is_running(false),
          trim_value(0),
          start(std::chrono::steady_clock::now()),
          set_millis_value(0) {}

    // Control mock behavior
    void setCurrentTime(const DateTime& dt) {
        set_time_value = dt;
    }
    
    void advanceTime(const TimeSpan& span) {
        set_time_value = set_time_value + span;
    }
    
    int8_t getTrimValue() const {
        return trim_value;
    }

    void set_millis(uint32_t millis) {
        set_millis_value = millis;
    }

    void set_proper_millis() {
        uint64_t total_millis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();
        
        set_millis_value = static_cast<uint32_t>(total_millis & 0xFFFFFFFFULL);
    }

    void set_time(const DateTime& dt) noexcept override {
        set_time_value = dt;
    }

    uint32_t millis() const noexcept override {
        return set_millis_value;
    }

    DateTime now() const noexcept override {
        return set_time_value;
    }

    bool begin()  {
        is_running = true;
        return true;
    }

    operator bool() const {
        return is_running;
    }
};

#endif // MOCK_CLOCK_H