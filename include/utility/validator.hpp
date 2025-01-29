#ifndef VALIDATOR_HPP
#define VALIDATOR_HPP

#include "utility.hpp"

#include <string>

struct validator {
public:
    explicit validator(const std::string &line);

    void validate_end_of_line(std::stringstream &sstream) const;

    [[nodiscard]] std::size_t validate_positive_integer(const std::string &value) const;

    [[nodiscard]] my_time validate_time(const std::string &time) const;

    [[nodiscard]] input_event_type validate_operation_type(const std::string &id) const;

    void validate_client_name(const std::string &name) const;

    void validate_table_number(std::size_t table_number, std::size_t number_of_tables) const;

    void validate_events_chronological_order(my_time previous_time, my_time curren_time) const;

private:
    std::string m_line;
};

#endif // VALIDATOR_HPP
