#include <iostream>

#include "executor.hpp"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Wrong number of arguments, expected <input-file>" << std::endl;
        return 1;
    }
    std::string inputfile = argv[1];
    try {
        Executor executor();
        return executor.run(inputfile);
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
}