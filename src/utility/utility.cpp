#include "utility.hpp"

#include <stdexcept>

my_time::my_time(const std::string &time) {
    m_hours = (time[0] - '0') * 10 + time[1] - '0';
    m_minutes = (time[3] - '0') * 10 + time[4] - '0';
}

my_time::my_time(std::int32_t hours, std::int32_t minutes)
    : m_minutes(minutes % 60), m_hours(hours + minutes / 60) {}

std::string my_time::to_string() const {
    if (!(0 <= m_hours && m_hours <= 23 && 0 <= m_minutes && m_minutes <= 59)) {
        throw std::domain_error("time is out of bounds: m_hours: " + std::to_string(m_hours) +
                                ", m_minutes: " + std::to_string(m_minutes));
    }
    char converted_time[6] = {0};
    std::snprintf(converted_time, sizeof(converted_time), "%02d:%02d", m_hours, m_minutes);
    return converted_time;
}

std::int32_t my_time::minutes() const { return m_minutes; }

std::int32_t my_time::hours() const { return m_hours; }

std::int32_t my_time::rounded_up_number_of_hours() const { return m_hours + (m_minutes != 0); }

bool my_time::operator<(const my_time &rhs) const {
    return m_hours < rhs.m_hours || (m_hours == rhs.m_hours && m_minutes < rhs.m_minutes);
}

my_time my_time::operator-(const my_time &rhs) const {
    my_time result{m_hours - rhs.m_hours, m_minutes - rhs.m_minutes};
    if (result.m_minutes < 0) {
        result.m_hours--;
        result.m_minutes += 60;
    }
    return result;
}

my_time my_time::operator+(const my_time &rhs) const {
    my_time result{m_hours + rhs.m_hours, m_minutes + rhs.m_minutes};
    if (result.m_minutes >= 60) {
        result.m_hours++;
        result.m_minutes -= 60;
    }
    return result;
}

my_time &my_time::operator+=(const my_time &rhs) {
    m_minutes += rhs.m_minutes;
    m_hours += rhs.m_hours;
    if (m_minutes >= 60) {
        m_hours++;
        m_minutes -= 60;
    }
    return *this;
}
