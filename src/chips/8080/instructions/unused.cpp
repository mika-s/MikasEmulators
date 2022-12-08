#include "crosscutting/memory/next_word.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/string_util.h"
#include <iostream>
#include <string>

namespace emu::i8080 {

using emu::memory::NextWord;
using emu::util::string::hexify;
using emu::util::string::hexify_wo_0x;

/**
 * Unused instruction of size 1
 * @param opcode is the opcode of the instruction
 * @param cycles is the number of cycles variable, which will be mutated
 */
void unused_1(u8 opcode, cyc& cycles)
{
    std::cout << "Opcode not supposed to be used: " << hexify(opcode) << "\n";

    cycles = 0;
}

/**
 * Unused instruction of size 3
 * @param opcode is the opcode of the instruction
 * @param cycles is the number of cycles variable, which will be mutated
 */
void unused_3(u8 opcode, u16& pc, cyc& cycles)
{
    unused_1(opcode, cycles);
    pc += 2;
}

void print_unused_nop(std::ostream& ostream)
{
    ostream << "*NOP";
}

void print_unused_jmp(std::ostream& ostream, NextWord const& args)
{
    ostream << "*JMP "
            << hexify_wo_0x(args.sarg)
            << hexify_wo_0x(args.farg);
}

void print_unused_ret(std::ostream& ostream)
{
    ostream << "*RET";
}

void print_unused_call(std::ostream& ostream, NextWord const& args)
{
    ostream << "*CALL "
            << hexify_wo_0x(args.sarg)
            << hexify_wo_0x(args.farg);
}
}
