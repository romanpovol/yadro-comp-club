#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <stdexcept>

struct error_in_input_file_exception : std::runtime_error {
    explicit error_in_input_file_exception(const std::string& line_with_error);
};

#endif  // EXCEPTIONS_HPP
