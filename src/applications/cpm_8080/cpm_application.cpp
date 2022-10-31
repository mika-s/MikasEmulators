#include "cpm_application.h"
#include "cpm_application_session.h"
#include "crosscutting/util/byte_util.h"
#include "crosscutting/util/file_util.h"

namespace emu::applications::cpm::i8080 {

    using emu::util::byte::to_u16;
    using emu::util::file::read_file_into_vector;

    CpmApplication::CpmApplication(const std::string &file) {
        load_file(file);
    }

    std::unique_ptr<Session> CpmApplication::new_session() {
        return std::make_unique<CpmApplicationSession>(m_loaded_file, m_memory);
    }

    void CpmApplication::load_file(const std::string &file) {
        m_loaded_file = file;

        const std::vector<u8> initial_offset = create_initial_offset();
        const std::vector<u8> rom = read_file_into_vector(file);
        const std::vector<u8> work_ram = create_work_ram(UINT16_MAX - initial_offset.size() - rom.size() + 1);

        m_memory.add(initial_offset);
        m_memory.add(rom);
        m_memory.add(work_ram);

        patch_program(m_memory);
    }

    std::vector<u8> CpmApplication::create_initial_offset() {
        std::vector<u8> offset;
        const int size = 0x100; // Because the initial "ORG 00100H" offset in the source

        offset.reserve(size);
        for (int i = 0; i < size; ++i) {
            offset.push_back(0);
        }

        return offset;
    }

    std::vector<u8> CpmApplication::create_work_ram(size_t size) {
        std::vector<u8> work_ram;

        work_ram.reserve(size);
        for (size_t i = 0; i < size; ++i) {
            work_ram.push_back(0);
        }

        return work_ram;
    }

    void CpmApplication::patch_program(EmulatorMemory &program) {
        program.write(0x0000, 0xd3);
        program.write(0x0005, 0xd3);
        program.write(0x0006, 0x01);
        program.write(0x0007, 0xc9);
    }
}
