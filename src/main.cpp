#include <iostream>
#include <fstream>

#include "mtemu.hpp"

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << *argv << " <input.mte> <output prefix>\n";
        return 1;
    }
    mtemu::MtEmuProgram program;
    std::ifstream input(argv[1], std::ifstream::in | std::ifstream::binary);
    input >> program;
    program.toBin(argv[2]);
    return 0;
}
