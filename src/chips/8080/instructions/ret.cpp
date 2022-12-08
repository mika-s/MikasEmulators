#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/typedefs.h"
#include "doctest.h"
#include "instruction_util.h"
#include <iostream>
#include <vector>

namespace emu::i8080 {
/**
 * Return from subroutine
 * <ul>
 *   <li>Size: 1</li>
 *   <li>Cycles: 3</li>
 *   <li>States: 10</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param pc is the program counter, which will be mutated
 * @param sp is the stack pointer, which will be mutated
 * @param memory is the memory, which will be mutated
 * @param flag_reg is the flag register
 * @param cycles is the number of cycles variable, which will be mutated
 */
void ret(u16& pc, u16& sp, EmulatorMemory<u16, u8> const& memory, cyc& cycles)
{
    execute_return(pc, sp, memory);

    cycles = 10;
}

void print_ret(std::ostream& ostream)
{
    ostream << "RET";
}

TEST_CASE("8080: RET")
{
    cyc cycles = 0;
    u16 pc = 0x100f;
    u16 sp = 0;
    EmulatorMemory<u16, u8> memory;
    memory.add(std::vector<u8> { 0xab, 0x01, 0x02, 0x03, 0x04, 0x05 });

    SUBCASE("should pop PC off the stack")
    {
        ret(pc, sp, memory, cycles);

        CHECK_EQ(0x01ab, pc);
    }

    SUBCASE("should use 10 cycles")
    {
        cycles = 0;

        ret(pc, sp, memory, cycles);

        CHECK_EQ(10, cycles);
    }
}
}
