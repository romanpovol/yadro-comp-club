cmake_minimum_required(VERSION 3.22)
project(yadro_comp_club)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wextra -fsanitize=undefined)
add_link_options(-Wall -Wextra -fsanitize=undefined)

include_directories(include)
include_directories(include/utility)

add_executable(yadro_comp_club
        main.cpp
        src/parser.cpp
        src/executor.cpp
        src/utility/exceptions.cpp
        src/utility/validator.cpp
        src/utility/utility.cpp
        src/utility/data.cpp
)

add_executable(yadro_comp_club_tests
        src/parser.cpp
        src/executor.cpp
        src/utility/exceptions.cpp
        src/utility/validator.cpp
        src/utility/utility.cpp
        src/utility/data.cpp
        tests/my_time_test.cpp
        tests/validator_test.cpp
        tests/parser_test.cpp
        tests/doctest_main.cpp
        tests/executor_test.cpp
)
