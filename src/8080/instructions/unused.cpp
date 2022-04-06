#include <cstdint>
#include <iostream>
#include "8080/next_word.h"
#include "crosscutting/string_util.h"

namespace emu::cpu8080 {
    /**
     * Unused instruction of size 1
     * @param opcode is the opcode of the instruction
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void unused_1(std::uint8_t opcode, unsigned long &cycles) {
        std::cout << "Opcode not supposed to be used: " << emu::util::string::hexify(opcode) << "\n";

        cycles = 0;
    }

    /**
     * Unused instruction of size 3
     * @param opcode is the opcode of the instruction
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void unused_3(std::uint8_t opcode, std::uint16_t &pc, unsigned long &cycles) {
        unused_1(opcode, cycles);
        pc += 2;
    }

    void print_unused_nop(std::ostream& ostream) {
        ostream << "*NOP";
    }

    void print_unused_jmp(std::ostream& ostream, const NextWord &args) {
        ostream << "*JMP "
                << emu::util::string::hexify_wo_0x(args.sarg)
                << emu::util::string::hexify_wo_0x(args.farg);
    }

    void print_unused_ret(std::ostream& ostream) {
        ostream << "*RET";
    }

    void print_unused_call(std::ostream& ostream, const NextWord &args) {
        ostream << "*CALL "
                << emu::util::string::hexify_wo_0x(args.sarg)
                << emu::util::string::hexify_wo_0x(args.farg);
    }
}
