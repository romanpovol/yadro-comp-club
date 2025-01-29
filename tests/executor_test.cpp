#include <sstream>
#include "doctest.h"
#include "executor.hpp"
#include "parser.hpp"
#include "exceptions.hpp"

inline const std::vector<std::size_t> correct_tests = {0, 1, 6, 7, 8};
inline const std::vector<std::size_t> incorrect_tests = {2, 3, 4, 5, 9, 10, 11, 12, 13, 14};

TEST_CASE("check executor") {
    for (const auto &test_number: correct_tests) {
        std::stringstream output;
        executor executor("../tests/input/" + std::to_string(test_number) + ".in", output);
        executor.run();
        std::ifstream answer_file("../tests/output/" + std::to_string(test_number) + ".out");
        std::stringstream answer;
        answer << answer_file.rdbuf();
        answer_file.close();
        CHECK(answer.str() == output.str());
    }

    for (const auto &test_number: incorrect_tests) {
        std::stringstream output;
        std::string line_with_error;
        try {
            executor executor("../tests/input/" + std::to_string(test_number) + ".in", output);
            executor.run();
        } catch (error_in_input_file_exception& e) {
            line_with_error = e.what();
        }
        std::ifstream answer_file("../tests/output/" + std::to_string(test_number) + ".out");
        std::stringstream answer;
        answer << answer_file.rdbuf();
        answer_file.close();
        CHECK(answer.str() == line_with_error);
    }
}
