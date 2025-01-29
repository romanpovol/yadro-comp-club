#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>

#include "data.hpp"

struct executor {
public:
    explicit executor(const std::string &input_file, std::ostream &ostream = std::cout);

    int run();

private:
    input_data m_input_data;
    std::queue<std::string> m_clients_queue;
    std::unordered_map<std::string, input_event_type> m_clients_status;

    std::unordered_set<std::size_t> m_free_table_numbers;
    std::vector<std::string> m_tables;
    std::unordered_map<std::string, std::size_t> m_table_number_by_client;
    std::ostream &m_ostream;

    struct table {
        std::size_t m_revenue{};
        my_time m_how_long_was_table_taken{};
        std::optional<my_time> m_time_when_table_became_taken;
    };
    std::vector<table> m_information_about_tables;

    void print_input_event(const event &event);

    void print_error(my_time time, const std::string &error_message);

    void client_arrived(const event &event);

    void client_sat(const event &event);

    void update_information_about_table(size_t number, my_time time);

    void client_waiting(const event &event);

    void client_left_club(const std::string &client_name, my_time time);

    void client_left_from_table(const event &event);

    void client_from_queue_sat(my_time time, size_t number);
};
