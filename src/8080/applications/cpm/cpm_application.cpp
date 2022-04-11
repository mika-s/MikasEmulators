#include <string>
#include "cpm_application.h"
#include "8080/applications/cpm/cpm_application_session.h"
#include "crosscutting/byte_util.h"
#include "crosscutting/file_util.h"

namespace emu::cpu8080::applications::cpm {

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

        m_memory.add(create_initial_offset());
        m_memory.add(read_file_into_vector(file));
        m_memory.add(create_work_ram(UINT16_MAX + 1 - m_memory.size()));
        patch_program(m_memory);
    }

    std::vector<std::uint8_t> CpmApplication::create_initial_offset() {
        std::vector<std::uint8_t> offset;
        const int size = 0x100; // Because the initial "ORG 00100H" offset in the source

        offset.reserve(size);
        for (int i = 0; i < size; ++i) {
            offset.push_back(0);
        }

        return offset;
    }

    std::vector<std::uint8_t> CpmApplication::create_work_ram(size_t size) {
        std::vector<std::uint8_t> work_ram;

        work_ram.reserve(size);
        for (size_t i = 0; i < size; ++i) {
            work_ram.push_back(0);
        }

        return work_ram;
    }

    void CpmApplication::patch_program(emu::cpu8080::EmulatorMemory &program) {
        program[0x0000] = 0xd3;
        program[0x0005] = 0xd3;
        program[0x0006] = 0x01;
        program[0x0007] = 0xc9;
    }
}
