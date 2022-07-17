#include "z80/flags.h"
#include "z80/next_word.h"
#include "z80/instructions/instruction_util.h"
#include "crosscutting/util/byte_util.h"

namespace emu::z80 {

    using emu::util::byte::first_byte;
    using emu::util::byte::second_byte;
    using emu::util::byte::to_u16;

    void add_to_register(u8 &acc_reg, u8 value, bool cf, Flags &flag_reg) {
        const u8 previous = acc_reg;
        const u8 to_add = value + (cf ? 1 : 0);
        acc_reg += to_add;

        flag_reg.handle_carry_flag(previous, value, cf);
        flag_reg.handle_zero_flag(acc_reg);
        flag_reg.handle_overflow_flag(previous, value, cf);
        flag_reg.handle_sign_flag(acc_reg);
        flag_reg.handle_half_carry_flag(previous, value, cf);
    }

    void sub_from_register(u8 &acc_reg, u8 value, bool cf, Flags &flag_reg) {
        const u8 previous = acc_reg;
        const u8 to_subtract = value + (cf ? 1 : 0);
        acc_reg -= to_subtract;

        flag_reg.handle_borrow_flag(previous, value, cf);
        flag_reg.handle_zero_flag(acc_reg);
        flag_reg.handle_overflow_flag(previous, -value, cf);    // TODO: Fix the overflow flag for sub/sbc
        flag_reg.handle_sign_flag(acc_reg);
        flag_reg.handle_half_borrow_flag(previous, value, cf);
    }

    void execute_call(u16 &pc, u16 &sp, EmulatorMemory &memory, const NextWord &args) {
        memory[--sp] = second_byte(pc);
        memory[--sp] = first_byte(pc);

        pc = to_u16(args.sarg, args.farg);
    }

    void execute_call(u16 &pc, u16 &sp, EmulatorMemory &memory, u8 farg, u8 sarg) {
        memory[--sp] = second_byte(pc);
        memory[--sp] = first_byte(pc);

        pc = to_u16(sarg, farg);
    }

    void execute_return(u16 &pc, u16 &sp, const EmulatorMemory &memory) {
        const u8 sarg = memory[sp++];
        const u8 farg = memory[sp++];
        pc = to_u16(farg, sarg);
    }
}
