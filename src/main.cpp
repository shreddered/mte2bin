#include <iostream>
#include <fstream>

#include "mtemu.hpp"

int main(int argc, char** argv) {
    mtemu::MtEmuProgram program;
    std::ifstream input(argv[1], std::ifstream::in | std::ifstream::binary);
    input >> program;
    std::ofstream output(argv[2], std::ofstream::out | std::ofstream::binary);
    output << program;
    return 0;
}
