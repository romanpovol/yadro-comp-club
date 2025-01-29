#include <sstream>
#include "parser.hpp"
#include "validator.hpp"
#include "exceptions.hpp"

parser::parser(const std::string &filename) : m_filestream(filename) {}

parser::~parser() {
    m_filestream.close();
}

input_data parser::parse() {
    input_data data;
    data.m_number_of_tables = parse_number_of_tables();
    auto [opening_time, closing_time] = parse_working_hours();
    data.m_opening_time = opening_time;
    data.m_closing_time = closing_time;
    data.m_price_per_hour = parse_price_per_hour();
    data.m_events = parse_events(data.m_number_of_tables);
    return data;
}

std::size_t parser::parse_number_of_tables() {
    std::size_t line_number = 1;
    std::string line;
    if (std::getline(m_filestream, line)) {
        validator validator(line);
        std::stringstream sstream(line);

        std::string str_number_of_tables;
        std::size_t number_of_tables;

        if (std::getline(sstream, str_number_of_tables, WORD_ENDLINE)) {
            number_of_tables = validator.validate_positive_integer(str_number_of_tables);
        } else {
            throw error_in_input_file_exception(line);
        }

        validator.validate_end_of_line(sstream);
        return number_of_tables;

    } else {
        throw std::runtime_error("Unable to read " + std::to_string(line_number) + " line");
    }
}

parser::working_hours parser::parse_working_hours() {
    int line_number = 2;
    std::string input_line;
    if (std::getline(m_filestream, input_line)) {
        validator validator(input_line);
        std::stringstream sstream(input_line);

        std::string opening_time, closing_time;
        working_hours working_hours{};

        if (std::getline(sstream, opening_time, WORD_DELIMETER)) {
            working_hours.opening_time = validator.validate_time(opening_time);
        } else {
            throw error_in_input_file_exception(input_line);
        }

        if (std::getline(sstream, closing_time, WORD_ENDLINE)) {
            working_hours.closing_time = validator.validate_time(closing_time);
        } else {
            throw error_in_input_file_exception(input_line);
        }

        validator.validate_end_of_line(sstream);
        return working_hours;
    } else {
        throw std::runtime_error("Unable to read " + std::to_string(line_number) + " line");
    }
}

std::size_t parser::parse_price_per_hour() {
    std::size_t line_number = 3;
    std::string line;
    if (std::getline(m_filestream, line)) {
        validator validator(line);
        std::stringstream sstream(line);

        std::string str_price_per_hour;
        std::size_t price_per_hour;

        if (std::getline(sstream, str_price_per_hour, WORD_ENDLINE)) {
            price_per_hour = validator.validate_positive_integer(str_price_per_hour);
        } else {
            throw error_in_input_file_exception(line);
        }

        validator.validate_end_of_line(sstream);
        return price_per_hour;
    } else {
        throw std::runtime_error("Unable to read " + std::to_string(line_number) + " line");
    }
}

std::vector<event> parser::parse_events(std::size_t number_of_tables) {
    std::string line;
    std::vector<event> events;
    while (std::getline(m_filestream, line)) {
        validator validator(line);
        std::stringstream sstream(line);

        std::string str_event_time, str_event_type, event_client_name, str_event_table_number;
        my_time event_time{};
        input_event_type event_type;
        std::optional<std::size_t> event_table_number{};

        if (std::getline(sstream, str_event_time, WORD_DELIMETER)) {
            event_time = validator.validate_time(str_event_time);
        } else {
            throw error_in_input_file_exception(line);
        }

        if (std::getline(sstream, str_event_type, WORD_DELIMETER)) {
            event_type = validator.validate_operation_type(str_event_type);
        } else {
            throw error_in_input_file_exception(line);
        }

        if (event_type == input_event_type::client_sat) {
            if (std::getline(sstream, event_client_name, WORD_DELIMETER)) {
                validator.validate_client_name(event_client_name);
            } else {
                throw error_in_input_file_exception(line);
            }

            if (std::getline(sstream, str_event_table_number, WORD_ENDLINE)) {
                event_table_number = validator.validate_positive_integer(str_event_table_number);
                validator.validate_table_number(event_table_number.value(), number_of_tables);
            } else {
                throw error_in_input_file_exception(line);
            }
        } else {
            if (std::getline(sstream, event_client_name, WORD_ENDLINE)) {
                validator.validate_client_name(event_client_name);
            } else {
                throw error_in_input_file_exception(line);
            }
        }

        validator.validate_end_of_line(sstream);
        if (!events.empty()) {
            validator.validate_events_chronological_order(events.back().m_time, event_time);
        }

        events.emplace_back(event_time, event_type, event_client_name, event_table_number);
    }
    return events;
}
