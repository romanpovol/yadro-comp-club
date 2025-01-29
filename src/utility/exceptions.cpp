#include "exceptions.hpp"

error_in_input_file_exception::error_in_input_file_exception(const std::string& line_with_error) :
        std::runtime_error(line_with_error) {}