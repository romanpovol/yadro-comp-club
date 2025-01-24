cmake_minimum_required(VERSION 3.28)
project(yadro_comp_club)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wextra -fsanitize=address -fsanitize=undefined)
add_link_options(-Wall -Wextra -fsanitize=address -fsanitize=undefined)

include_directories(include)

add_executable(yadro_comp_club
        main.cpp
        src/parser.cpp
        src/executor.cpp
        src/utility/exceptions.cpp
        src/utility/validator.cpp
        src/utility/utility.cpp
)
