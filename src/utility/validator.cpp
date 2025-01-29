#include "validator.hpp"

#include <regex>

#include "exceptions.hpp"

validator::validator(const std::string &line) : m_line(line) {}

void validator::validate_end_of_line(std::stringstream &sstream) const {
    if (sstream.peek() != EOF || sstream.peek() == '\n') {
        throw error_in_input_file_exception(m_line);
    }
}

std::size_t validator::validate_positive_integer(const std::string &value) const {
    if (value.empty() || value[0] < '1' || value[0] > '9' ||
        !std::all_of(value.begin(), value.end(), [](char c) {
            return '0' <= c && c <= '9';
        })) {
        throw error_in_input_file_exception(m_line);
    }
    return stoul(value);
}

void validator::validate_client_name(const std::string &name) const {
    std::regex pattern("(^[a-z0-9_-]+$)");
    if (name.empty() || !std::regex_match(name, pattern)) {
        throw error_in_input_file_exception(m_line);
    }
}

my_time validator::validate_time(const std::string &time) const {
    std::regex pattern("(^(0[0-9]|1[0-9]|2[0-3]):([0-5][0-9])$)");
    if (time.empty() || !std::regex_match(time, pattern)) {
        throw error_in_input_file_exception(m_line);
    }
    return my_time(time);
}

void validator::validate_table_number(std::size_t table_number,
                                      std::size_t number_of_tables) const {
    if (table_number < 1 || table_number > number_of_tables) {
        throw error_in_input_file_exception(m_line);
    }
}

input_event_type validator::validate_operation_type(const std::string &type) const {
    if (type.size() != 1 || (type[0] - '0') < static_cast<int>(input_event_type::client_arrived) ||
        (type[0] - '0') > static_cast<int>(input_event_type::client_left)) {
        throw error_in_input_file_exception(m_line);
    }
    return static_cast<input_event_type>(stoul(type));
}

void validator::validate_events_chronological_order(
        my_time previous_time,
        my_time curren_time) const {
    if (curren_time < previous_time) {
        throw error_in_input_file_exception(m_line);
    }
}
