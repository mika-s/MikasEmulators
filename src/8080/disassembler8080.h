#ifndef MIKA_EMULATORS_8080_DISASSEMBLER_H
#define MIKA_EMULATORS_8080_DISASSEMBLER_H

#include <cstdint>
#include <string>
#include "next_byte.h"
#include "next_word.h"
#include "emulator_memory.h"

namespace emu::cpu8080 {

    class Disassembler8080 {
    public:
        explicit Disassembler8080(emu::cpu8080::EmulatorMemory &memory);

        void disassemble();

    private:
        emu::cpu8080::EmulatorMemory &memory;
        std::size_t memory_size;

        std::uint16_t pc;
        std::uint8_t opcode;

        void print_next_instruction();

        NextByte get_next_byte();

        NextWord get_next_word();
    };
}

#endif //MIKA_EMULATORS_8080_DISASSEMBLER_H
