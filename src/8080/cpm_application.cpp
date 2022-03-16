#include <string>
#include <iostream>
#include "byte_util.h"
#include "file_util.h"
#include "cpm_application.h"

namespace emu::cpu8080 {

    using emu::util::file::read_file_into_vector;

    CpmApplication::CpmApplication(const std::string& file) :
            cpu(nullptr),
            finished(false) {
        load_file(file);
    }

    CpmApplication::~CpmApplication() {
        delete cpu;
    }

    void CpmApplication::load_file(const std::string& file) {
        loaded_file = file;

        memory.add(create_initial_offset());
        memory.add(read_file_into_vector(file));
        memory.add(create_work_ram(UINT16_MAX + 1 - memory.size()));
        patch_program(memory);

        const std::uint16_t initial_pc = 0x100;

        cpu = new Cpu(memory, initial_pc);

        cpu->add_out_observer(*this);
    }

    void CpmApplication::run() {
        std::cout << "--------------- Starting " << loaded_file  << " ---------------\n\n";
        cpu->start();

        while (cpu->can_run_next_instruction() && !finished) {
            cpu->next_instruction();
        }

        cpu->stop();
        std::cout << "\n\n--------------- Finished " << loaded_file  << " ---------------\n\n";
    }

    void CpmApplication::out_changed(std::uint8_t port) {
        if (port == 0) {
            finished = true;
        } else if (port == 1) {
            std::uint8_t operation = cpu->C();

            if (operation == C_WRITE) {
                c_write(cpu->E());
            } else if (operation == C_WRITESTR) {
                c_writestr(cpu->Memory(), emu::util::byte::to_uint16_t(cpu->D(), cpu->E()));
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
