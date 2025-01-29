#include <sstream>
#include "doctest.h"
#include "executor.hpp"
#include "parser.hpp"
#include "exceptions.hpp"

extern const std::vector<std::size_t> correct_tests;
extern const std::vector<std::size_t> incorrect_tests;

TEST_CASE("check parser") {
    for (const auto &test_number: correct_tests) {
        CHECK_NOTHROW(parser("../tests/input/" + std::to_string(test_number) + ".in").parse());
    }
    for (const auto &test_number: incorrect_tests) {
        CHECK_THROWS(parser("../tests/input/" + std::to_string(test_number) + ".in").parse());
    }
}
