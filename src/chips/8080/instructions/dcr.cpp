#include "chips/8080/flags.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/typedefs.h"
#include "doctest.h"
#include <iostream>

namespace emu::i8080 {

    using emu::memory::EmulatorMemory;

    void dcr(u8 &reg, Flags &flag_reg) {
        const u8 previous = reg;
        --reg;

        flag_reg.handle_zero_flag(reg);
        flag_reg.handle_parity_flag(reg);
        flag_reg.handle_sign_flag(reg);
        flag_reg.handle_aux_borrow_flag(previous, 1, false);
    }

    /**
     * Decrement register
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 5</li>
     *   <li>Condition bits affected: auxiliary carry, zero, sign, parity</li>
     * </ul>
     *
     * @param reg is the register to decrement, which will be mutated
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void dcr_r(u8 &reg, Flags &flag_reg, cyc &cycles) {
        dcr(reg, flag_reg);

        cycles = 5;
    }

    /**
     * Decrement value in memory
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 0</li>
     *   <li>Condition bits affected: auxiliary carry, zero, sign, parity</li>
     * </ul>
     *
     * @param memory is the memory, which will be mutated
     * @param address is the address of value in memory to decrement
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     * @param is_memory_involved is true if memory is involved, either written or read
     */
    void dcr_m(EmulatorMemory &memory, u16 address, Flags &flag_reg, cyc &cycles) {
        u8 value = memory.read(address);

        dcr(value, flag_reg);

        memory.write(address, value);

        cycles = 10;
    }

    void print_dcr(std::ostream &ostream, const std::string &reg) {
        ostream << "DCR "
                << reg;
    }

    TEST_CASE("8080: DCR") {
        cyc cycles = 0;

        SUBCASE("should decrease register or memory") {
            u8 reg = 10;
            Flags flag_reg;

            dcr_r(reg, flag_reg, cycles);
            CHECK_EQ(9, reg);

            dcr_r(reg, flag_reg, cycles);
            CHECK_EQ(8, reg);

            dcr_r(reg, flag_reg, cycles);
            CHECK_EQ(7, reg);

            dcr_r(reg, flag_reg, cycles);
            CHECK_EQ(6, reg);

            dcr_r(reg, flag_reg, cycles);
            CHECK_EQ(5, reg);
        }

        SUBCASE("should not affect the carry flag") {
            u8 reg = 0;
            Flags flag_reg;

            CHECK_EQ(false, flag_reg.is_carry_flag_set());

            dcr_r(reg, flag_reg, cycles);

            CHECK_EQ(255, reg);
            CHECK_EQ(false, flag_reg.is_carry_flag_set());
        }

        SUBCASE("should set correct aux carry flag") {
            u8 reg = 15;
            Flags flag_reg;

            CHECK_EQ(false, flag_reg.is_aux_carry_flag_set());

            dcr_r(reg, flag_reg, cycles);

            CHECK_EQ(14, reg);
            CHECK_EQ(true, flag_reg.is_aux_carry_flag_set());
        }

        SUBCASE("should set correct parity in the parity flag") {
            u8 reg = 5;
            Flags flag_reg;

            dcr_r(reg, flag_reg, cycles);
            CHECK_EQ(false, flag_reg.is_parity_flag_set());

            dcr_r(reg, flag_reg, cycles);
            CHECK_EQ(true, flag_reg.is_parity_flag_set());
        }

        SUBCASE("should set correct value in the zero flag") {
            u8 reg = 2;
            Flags flag_reg;

            dcr_r(reg, flag_reg, cycles);
            CHECK_EQ(false, flag_reg.is_zero_flag_set());

            dcr_r(reg, flag_reg, cycles);
            CHECK_EQ(true, flag_reg.is_zero_flag_set());
        }

        SUBCASE("should set the sign flag when going above 127") {
            u8 reg = 0;
            Flags flag_reg;

            CHECK_EQ(false, flag_reg.is_sign_flag_set());

            dcr_r(reg, flag_reg, cycles);

            CHECK_EQ(true, flag_reg.is_sign_flag_set());
        }

        SUBCASE("should use 5 cycles when memory is not involved") {
            cycles = 0;
            Flags flag_reg;
            u8 reg = 0xe;

            dcr_r(reg, flag_reg, cycles);

            CHECK_EQ(5, cycles);
        }

        SUBCASE("should use 10 cycles when memory is involved") {
            cycles = 0;
            Flags flag_reg;
            EmulatorMemory memory;
            memory.add({0x10});
            u16 address = 0x0000;

            dcr_m(memory, address, flag_reg, cycles);

            CHECK_EQ(10, cycles);
        }
    }
}
