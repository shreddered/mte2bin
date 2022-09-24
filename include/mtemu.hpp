#ifndef ASVT_INCLUDE_MTEMU_HPP_
#define ASVT_INCLUDE_MTEMU_HPP_

#include <cstdint>
#include <istream>
#include <ostream>
#include <vector>
#include <string>

namespace mtemu {

class MtEmuProgramHeader;
class MtEmuProgram;

} // namespace mtemu

std::istream& operator>>(std::istream& is, mtemu::MtEmuProgramHeader& header);
std::istream& operator>>(std::istream& is, mtemu::MtEmuProgram& program);
std::ostream& operator<<(std::ostream& os, const mtemu::MtEmuProgram& program);

namespace mtemu {

class MtEmuProgramHeader {
public:
    MtEmuProgramHeader() noexcept = default;
    friend std::istream& ::operator>>(std::istream& is, mtemu::MtEmuProgramHeader& header);
    inline uint32_t size() const noexcept {
        return m_size;
    }
private:
    uint32_t m_magic;
    uint32_t m_size;
}; // class MtEmuProgramHeader

class MtEmuProgram {
public:
    MtEmuProgram() noexcept;
    friend std::istream& ::operator>>(std::istream& is, mtemu::MtEmuProgram& program);
    friend std::ostream& ::operator<<(std::ostream& os, const mtemu::MtEmuProgram& program);
    void toBin(std::string pathPrefix) const;
private:
    static const uint32_t COMMAND_SIZE = 6;
    MtEmuProgramHeader m_header;
    std::vector<uint64_t> m_commands;
}; // class MtEmuProgram
   
} // namespace mtemu

#endif // ASVT_INCLUDE_MTEMU_HPP_
