#include "data.hpp"

event::event(my_time time, input_event_type type, const std::string &client_name,
             std::optional<std::size_t> table_number)
    : m_time(time),
      m_type(type),
      m_client_name(client_name),
      m_table_number(table_number) {}
