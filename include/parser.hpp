#ifndef PARSER_H
#define PARSER_H

#include <fstream>

#include "data.hpp"
#include "utility.hpp"

static const char WORD_DELIMETER = ' ';
static const char WORD_ENDLINE = '\n';

struct parser {
public:
    explicit parser(const std::string &filename);

    ~parser();

    parser(const parser &other) = delete;
    parser(parser &&other) noexcept = default;
    parser &operator=(const parser &other) = delete;
    parser &operator=(parser &&other) noexcept = default;

    input_data parse();

private:
    std::ifstream m_filestream;

    std::size_t parse_number_of_tables();

    struct working_hours {
        my_time opening_time;
        my_time closing_time;
    };

    working_hours parse_working_hours();

    std::size_t parse_price_per_hour();

    std::vector<event> parse_events(std::size_t number_of_tables);
};

#endif  // PARSER_H