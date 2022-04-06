#include <string>
#include <iostream>
#include "cpm_application.h"
#include "crosscutting/byte_util.h"
#include "crosscutting/file_util.h"

namespace emu::cpu8080::applications::cpm {

    using emu::util::byte::to_u16;
    using emu::util::file::read_file_into_vector;

    CpmApplication::CpmApplication(const std::string &file)
            : m_is_finished(false) {
        load_file(file);
        setup_cpu();
    }

    void CpmApplication::load_file(const std::string &file) {
        m_loaded_file = file;

        m_memory.add(create_initial_offset());
        m_memory.add(read_file_into_vector(file));
        m_memory.add(create_work_ram(UINT16_MAX + 1 - m_memory.size()));
        patch_program(m_memory);
    }

    void CpmApplication::setup_cpu() {
        const std::uint16_t initial_pc = 0x100;

        m_cpu = std::make_unique<Cpu>(m_memory, initial_pc);

        m_cpu->add_out_observer(*this);
    }

    void CpmApplication::run() {
        std::cout << "--------------- Starting " << m_loaded_file << " ---------------\n\n";
        m_cpu->start();

        while (m_cpu->can_run_next_instruction() && !m_is_finished) {
            m_cpu->next_instruction();
        }

        m_cpu->stop();
        std::cout << "\n\n--------------- Finished " << m_loaded_file << " ---------------\n\n";
    }

    void CpmApplication::out_changed(std::uint8_t port) {
        if (port == 0) {
            m_is_finished = true;
        } else if (port == 1) {
            std::uint8_t operation = m_cpu->c();

            if (operation == C_WRITE) {
                c_write(m_cpu->e());
            } else if (operation == C_WRITESTR) {
                c_writestr(m_cpu->memory(), to_u16(m_cpu->d(), m_cpu->e()));
            }
        } else {
            throw std::runtime_error("Illegal output port");
        }
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

    void CpmApplication::c_write(std::uint8_t e) {
        std::cout << e;
    }

    void CpmApplication::c_writestr(const emu::cpu8080::EmulatorMemory &memory, std::uint16_t address) {
        do {
            std::cout << memory[address];
            address++;
        } while (memory[address] != '$');
    }
}
