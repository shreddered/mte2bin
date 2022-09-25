#include "mtemu.hpp"

#include <fstream>
#include <memory>
#include <stdexcept>

#include <x86intrin.h>

namespace mtemu {

uint8_t getPart(uint64_t command, uint64_t i) {
    // masks for `pext r64, r64, r64` instruction on x86 processors
    static uint64_t pextmask[] = { 0x00000000ff,   // B + D
                                   0x0000078f00,   // A + C0 + I0-I2
                                   0x0003700700,   // I3-I5 + I6-I8 + CA0-CA1
                                   0x03fc000000,   // CA2-CA3 + AR0-AR4 + AR5-AR6
                                   0x0c00000000 }; // AR6-AR7 + additional fields
    return (uint8_t) (_pext_u64(command, pextmask[i]) & 0xffu);
}

MtEmuProgram::MtEmuProgram() noexcept : m_header(), m_commands() {}

void MtEmuProgram::toBin(std::string pathPrefix) const {
    char num = '1';
    for (uint64_t i = 0; i < 5; ++i, ++num) {
        std::ofstream output{pathPrefix + num + ".bin", std::ofstream::out | std::ofstream::binary};
        for (uint64_t command : this->m_commands) {
            uint8_t part = getPart(command, i);
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
        program.m_commands.push_back(_bswap64(*(uint64_t*)(ptr + i) & 0xfffffffffffful) >> 16);
    return is;
}

std::ostream& operator<<(std::ostream& os, const mtemu::MtEmuProgram& program) {
    auto ptr = program.m_commands.data();
    for (size_t i = 0; i < program.m_commands.size(); ++i) {
        os.write((char*) (ptr + i), sizeof(uint64_t));
    }
    return os;
}
