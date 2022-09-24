#include "mtemu.hpp"

#include <fstream>
#include <memory>
#include <stdexcept>

#include <x86intrin.h>

namespace mtemu {

MtEmuProgram::MtEmuProgram() noexcept : m_header(), m_commands() {}

void MtEmuProgram::toBin(std::string pathPrefix) const {
    char num = '1';
    for (uint64_t i = 0; i < 6; ++i, ++num) {
        std::ofstream output{pathPrefix + num + ".bin", std::ofstream::out | std::ofstream::binary};
        for (uint64_t command : this->m_commands) {
            uint8_t part = (command >> (i * 8)) & 0xff;
            output.write((char*) &part, 1);
        }
    }
}

} // namespace mtemu

std::istream& operator>>(std::istream& is, mtemu::MtEmuProgramHeader& header) {
    is.read((char*) &header.m_magic, sizeof(uint32_t));
    if (header.m_magic != 0x4d45544d) // MTEM in little endian
        throw std::invalid_argument("Header error: wrong magic number");
    is.read((char*) &header.m_size, sizeof(uint32_t));
    header.m_size = _bswap(header.m_size);
    return is;
}

std::istream& operator>>(std::istream& is, mtemu::MtEmuProgram& program) {
    is >> program.m_header;
    // prepare array of commands
    program.m_commands.reserve(program.m_header.size());
    // precalculations for binary I/O
    // binary data size
    uint32_t len = program.m_header.size() * mtemu::MtEmuProgram::COMMAND_SIZE;
    // aligned buffer size
    uint32_t len_ = (len % 8) ? (len + 8) & ~7u : len;
    std::unique_ptr<char[]> buf(new char[len_]);
    is.read(buf.get(), len);
    char* ptr = buf.get();
    for (size_t i = 0; i < len; i += mtemu::MtEmuProgram::COMMAND_SIZE)
        program.m_commands.push_back(*(uint64_t*)(ptr + i) & 0xfffffffffffful);
    return is;
}

std::ostream& operator<<(std::ostream& os, const mtemu::MtEmuProgram& program) {
    auto ptr = program.m_commands.data();
    for (size_t i = 0; i < program.m_commands.size(); ++i) {
        os.write((char*) (ptr + i), sizeof(uint64_t));
    }
    return os;
}
