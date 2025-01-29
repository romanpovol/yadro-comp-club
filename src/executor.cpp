#include "executor.hpp"

#include <algorithm>

#include "parser.hpp"

executor::executor(const std::string &input_file, std::ostream &ostream) : m_ostream(ostream) {
    parser parser(input_file);
    m_input_data = parser.parse();
    m_tables.resize(m_input_data.m_number_of_tables + 1);
    for (std::size_t i = 1; i <= m_input_data.m_number_of_tables; i++) {
        m_free_table_numbers.insert(i);
    }
    m_information_about_tables.resize(m_input_data.m_number_of_tables + 1);
}

int executor::run() {
    m_ostream << m_input_data.m_opening_time.to_string() << '\n';
    for (auto const &event : m_input_data.m_events) {
        switch (event.m_type) {
            case input_event_type::client_arrived: {
                client_arrived(event);
                break;
            }
            case input_event_type::client_sat: {
                client_sat(event);
                break;
            }
            case input_event_type::client_waiting: {
                client_waiting(event);
                break;
            }
            case input_event_type::client_left: {
                client_left_from_table(event);
                break;
            }
            default: {
                // The code shouldn't come here because we check it in the validator.
                throw std::runtime_error("Invalid event type");
            }
        }
    }

    std::vector<std::string> clients_names;
    clients_names.reserve(m_clients_status.size());
    for (auto const &[client_name, status] : m_clients_status) {
        clients_names.emplace_back(client_name);
    }

    std::sort(clients_names.begin(), clients_names.end());
    for (auto const &client_name : clients_names) {
        client_left_club(client_name, m_input_data.m_closing_time);
    }

    m_ostream << m_input_data.m_closing_time.to_string() << '\n';
    for (std::size_t i = 1; i < m_information_about_tables.size(); i++) {
        std::size_t revenue = m_information_about_tables[i].m_revenue;
        my_time time_table_was_busy = m_information_about_tables[i].m_how_long_was_table_taken;
        m_ostream << i << ' ' << revenue << ' ' << time_table_was_busy.to_string() << '\n';
    }

    return 0;
}

void executor::print_input_event(const event &event) {
    m_ostream << event.m_time.to_string() << ' ' << static_cast<std::size_t>(event.m_type) << ' '
              << event.m_client_name;
    if (event.m_table_number.has_value()) {
        m_ostream << ' ' << event.m_table_number.value();
    }
    m_ostream << '\n';
}

void executor::print_error(my_time time, const std::string &error_message) {
    m_ostream << time.to_string() << ' ' << static_cast<std::size_t>(output_event_type::error)
              << ' ' << error_message << '\n';
}

void executor::client_left_club(const std::string &client_name, my_time time) {
    m_ostream << time.to_string() << ' '
              << static_cast<std::size_t>(output_event_type::client_left_club) << ' ' << client_name
              << '\n';

    if (m_table_number_by_client.find(client_name) != m_table_number_by_client.end()) {
        std::size_t previous_table = m_table_number_by_client[client_name];
        update_information_about_table(previous_table, time);
        m_tables[previous_table].clear();
        m_free_table_numbers.insert(previous_table);
    }
}

void executor::update_information_about_table(std::size_t number, my_time current_time) {
    if (m_information_about_tables[number].m_time_when_table_became_taken.has_value()) {
        // take diff between the moment when client sat down at the table and the current moment
        // (got up from the table)
        my_time time_when_table_begin_busy =
            m_information_about_tables[number].m_time_when_table_became_taken.value();
        my_time diff_time = current_time - time_when_table_begin_busy;

        // updating information about the table
        m_information_about_tables[number].m_how_long_was_table_taken += diff_time;
        m_information_about_tables[number].m_revenue +=
            diff_time.rounded_up_number_of_hours() * m_input_data.m_price_per_hour;
        m_information_about_tables[number].m_time_when_table_became_taken.reset();
    } else {
        m_information_about_tables[number].m_time_when_table_became_taken.emplace(current_time);
    }
}

void executor::client_arrived(const event &event) {
    print_input_event(event);

    // if the client comes during non-working hours
    if (event.m_time < m_input_data.m_opening_time || m_input_data.m_closing_time < event.m_time) {
        print_error(event.m_time, ERROR_NOT_OPEN_YET);
        return;
    }

    // if the client is already in the club
    if (m_clients_status.find(event.m_client_name) != m_clients_status.end()) {
        print_error(event.m_time, ERROR_YOU_SHALL_NOT_PASS);
        return;
    }

    m_clients_status[event.m_client_name] = input_event_type::client_arrived;
}

void executor::client_sat(const event &event) {
    print_input_event(event);

    // if such user does not exist
    if (m_clients_status.find(event.m_client_name) == m_clients_status.end()) {
        print_error(event.m_time, ERROR_CLIENT_UNKNOWN);
        return;
    }

    // if the table is taken
    if (!m_tables[event.m_table_number.value()].empty()) {
        print_error(event.m_time, ERROR_PLACE_IS_BUSY);
        return;
    }

    // if client was sitting at another table up to this point, then we must update the info for
    // this previous table
    if (m_table_number_by_client.find(event.m_client_name) != m_table_number_by_client.end()) {
        std::size_t previous_table_number = m_table_number_by_client[event.m_client_name];
        update_information_about_table(previous_table_number, event.m_time);
        m_tables[previous_table_number].clear();
        m_free_table_numbers.insert(previous_table_number);
    }

    // updating information for the table at which the client sat down
    update_information_about_table(event.m_table_number.value(), event.m_time);
    m_table_number_by_client[event.m_client_name] = event.m_table_number.value();
    m_tables[event.m_table_number.value()] = event.m_client_name;
    m_free_table_numbers.erase(event.m_table_number.value());

    m_clients_status[event.m_client_name] = input_event_type::client_sat;
}

void executor::client_waiting(const event &event) {
    print_input_event(event);

    // if there is an empty table
    if (!m_free_table_numbers.empty()) {
        print_error(event.m_time, ERROR_I_CAN_WAIT_NO_LONGER);
        return;
    }

    // if the client is already in the queue
    if (m_clients_status[event.m_client_name] == input_event_type::client_waiting) {
        return;
    }

    // if the queue size is equal to the number of tables in the club, client leaves
    if (m_clients_queue.size() == m_input_data.m_number_of_tables) {
        client_left_club(event.m_client_name, event.m_time);
        if (m_clients_status.find(event.m_client_name) != m_clients_status.end()) {
            m_clients_status.erase(event.m_client_name);
        }
        return;
    }

    m_clients_queue.push(event.m_client_name);
    m_clients_status[event.m_client_name] = input_event_type::client_waiting;
}

void executor::client_left_from_table(const event &event) {
    print_input_event(event);

    if (m_clients_status.find(event.m_client_name) == m_clients_status.end()) {
        print_error(event.m_time, ERROR_CLIENT_UNKNOWN);
        return;
    }

    if (m_table_number_by_client.find(event.m_client_name) == m_table_number_by_client.end()) {
        m_clients_status.erase(event.m_client_name);
        return;
    }

    std::size_t previous_table_number = m_table_number_by_client[event.m_client_name];
    update_information_about_table(previous_table_number, event.m_time);
    m_tables[previous_table_number].clear();
    m_free_table_numbers.insert(previous_table_number);
    m_table_number_by_client.erase(event.m_client_name);
    m_clients_status.erase(event.m_client_name);

    if (!m_clients_queue.empty()) {
        client_from_queue_sat(event.m_time, previous_table_number);
    }
}

void executor::client_from_queue_sat(my_time time, std::size_t table_number) {
    std::string client_from_queue = m_clients_queue.front();
    m_clients_queue.pop();
    m_ostream << time.to_string() << ' '
              << static_cast<std::size_t>(output_event_type::client_sat_down_at_vacant_table) << ' '
              << client_from_queue << ' ' << table_number << '\n';

    update_information_about_table(table_number, time);
    m_free_table_numbers.erase(table_number);
    m_table_number_by_client[client_from_queue] = table_number;
    m_tables[table_number] = client_from_queue;
    m_clients_status[client_from_queue] = input_event_type::client_sat;
}
