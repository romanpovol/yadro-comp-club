#include "validator.hpp"

#include <iostream>
#include <sstream>

#include "doctest.h"
#include "parser.hpp"

TEST_CASE("check line is over") {
    std::stringstream sstream("abc ab a ");
    validator validator("");
    CHECK_THROWS(validator.validate_end_of_line(sstream));
    std::string s;
    CHECK(std::getline(sstream, s, WORD_DELIMETER));
    CHECK_THROWS(validator.validate_end_of_line(sstream));

    CHECK(std::getline(sstream, s, WORD_DELIMETER));
    CHECK_THROWS(validator.validate_end_of_line(sstream));

    CHECK(std::getline(sstream, s, WORD_ENDLINE));
    CHECK_NOTHROW(validator.validate_end_of_line(sstream));
    CHECK_THROWS(validator.validate_client_name(s));

    CHECK_FALSE(std::getline(sstream, s, WORD_ENDLINE));
    CHECK_NOTHROW(validator.validate_end_of_line(sstream));
}

TEST_CASE("check time is correct") {
    validator validator("");
    CHECK_NOTHROW(static_cast<void>(validator.validate_time("00:00")));
    CHECK_NOTHROW(static_cast<void>(validator.validate_time("23:59")));
    CHECK_NOTHROW(static_cast<void>(validator.validate_time("01:30")));
    CHECK_NOTHROW(static_cast<void>(validator.validate_time("15:01")));
    CHECK_THROWS(static_cast<void>(validator.validate_time("00:60")));
    CHECK_THROWS(static_cast<void>(validator.validate_time("24:00")));
    CHECK_THROWS(static_cast<void>(validator.validate_time("23:75")));
    CHECK_THROWS(static_cast<void>(validator.validate_time("")));
}

TEST_CASE("check name is correct") {
    validator validator("");
    CHECK_NOTHROW(validator.validate_client_name("name"));
    CHECK_NOTHROW(validator.validate_client_name("a1so_ok_name"));
    CHECK_THROWS(validator.validate_client_name("Not_ok_name1"));
    CHECK_THROWS(validator.validate_client_name("not_ok_name$"));
    CHECK_THROWS(validator.validate_client_name("not_ok_name!"));
    CHECK_THROWS(validator.validate_client_name(""));
}

TEST_CASE("check table number is correct") {
    validator validator("");
    CHECK_NOTHROW(validator.validate_table_number(1, 1));
    CHECK_NOTHROW(validator.validate_table_number(4, 15));
    CHECK_THROWS(validator.validate_table_number(0, 15));
    CHECK_THROWS(validator.validate_table_number(16, 15));
}

TEST_CASE("check operation type is correct") {
    validator validator("");
    CHECK_NOTHROW(static_cast<void>(validator.validate_operation_type("1")));
    CHECK_NOTHROW(static_cast<void>(validator.validate_operation_type("2")));
    CHECK_NOTHROW(static_cast<void>(validator.validate_operation_type("3")));
    CHECK_NOTHROW(static_cast<void>(validator.validate_operation_type("4")));
    CHECK_THROWS(static_cast<void>(validator.validate_operation_type("0")));
    CHECK_THROWS(static_cast<void>(validator.validate_operation_type("5")));
}

TEST_CASE("check time is in chronological order") {
    validator validator("");
    CHECK_NOTHROW(validator.validate_events_chronological_order(my_time(0, 0), my_time(0, 0)));
    CHECK_NOTHROW(validator.validate_events_chronological_order(my_time(12, 59), my_time(13, 00)));
    CHECK_THROWS(validator.validate_events_chronological_order(my_time(1, 0), my_time(0, 0)));
    CHECK_THROWS(validator.validate_events_chronological_order(my_time(13, 0), my_time(12, 59)));
}
