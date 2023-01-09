#include "cpm_application.h"
#include "cpm_application_session.h"
#include "crosscutting/util/byte_util.h"
#include "crosscutting/util/file_util.h"
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace emu::misc {
class Session;
}

namespace emu::applications::cpm::z80 {

using emu::util::byte::to_u16;
using emu::util::file::read_file_into_vector;

CpmApplication::CpmApplication(std::string const& file)
{
    load_file(file);
}

std::unique_ptr<Session> CpmApplication::new_session()
{
    return std::make_unique<CpmApplicationSession>(m_loaded_file, m_memory);
}

std::vector<u8> create_empty_vector(std::size_t size)
{
    std::vector<u8> vec(size, 0);
    return vec;
}

void CpmApplication::load_file(std::string const& file)
{
    m_loaded_file = file;

    const std::vector<u8> rom = read_file_into_vector(file);

    m_memory.add(create_empty_vector(0x100)); // Because the initial "ORG 00100H" offset in the source
    m_memory.add(rom);
    m_memory.add(create_empty_vector(UINT16_MAX - 0x100 - rom.size()));

    patch_program(m_memory);

    assert(m_memory.size() == 0xffff);
}

/** Print results to the terminal
 *
 * @param program is the program in binary format, which will be mutated
 */
void CpmApplication::patch_program(EmulatorMemory<u16, u8>& program)
{
    // Finished with tests:
    program.write(0x0000, 0xd3); // OUT 0x00

    // BDOS call point (0x0005)
    program.write(0x0005, 0xd3); // OUT 0x01
    program.write(0x0006, 0x01);
    program.write(0x0007, 0xc9); // RET
}
}
