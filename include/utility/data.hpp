#ifndef DATA_HPP
#define DATA_HPP

#include <cstdint>
#include <cstdio>
#include <optional>
#include <string>
#include <vector>

#include "utility.hpp"

struct event {
    my_time m_time;
    input_event_type m_type;
    std::string m_client_name;
    std::optional<std::size_t> m_table_number;

    event(my_time event_time, input_event_type event_type, const std::string &client_name,
          std::optional<std::size_t> table_number);
};

struct input_data {
    std::size_t m_number_of_tables;
    my_time m_opening_time;
    my_time m_closing_time;
    std::size_t m_price_per_hour;
    std::vector<event> m_events;
};

#endif  // DATA_HPP
