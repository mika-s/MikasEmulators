#include <cstdint>
#include "byte_util.h"
#include "instruction_util.h"
#include "flags.h"
#include "next_word.h"

namespace emu::cpu8080 {
    void add_to_register(std::uint8_t &acc_reg, std::uint8_t value, bool cf, Flags &flag_reg) {
        const std::uint8_t previous = acc_reg;
        acc_reg += value + (cf ? 1 : 0);

        flag_reg.handle_carry_flag(previous, value + (cf ? 1 : 0));
        flag_reg.handle_zero_flag(acc_reg);
        flag_reg.handle_parity_flag(acc_reg);
        flag_reg.handle_sign_flag(acc_reg);
        flag_reg.handle_aux_carry_flag(previous, value, cf);
    }

    void sub_from_register(std::uint8_t &acc_reg, std::uint8_t value, bool cf, Flags &flag_reg) {
        const std::uint8_t previous = acc_reg;
        acc_reg -= (value + (cf ? 1 : 0));

        flag_reg.handle_borrow_flag(previous, value + (cf ? 1 : 0));
        flag_reg.handle_zero_flag(acc_reg);
        flag_reg.handle_parity_flag(acc_reg);
        flag_reg.handle_sign_flag(acc_reg);
        flag_reg.handle_aux_borrow_flag(previous, value, cf);
    }

    void execute_call(std::uint16_t &pc, std::uint16_t &sp, emu::cpu8080::EmulatorMemory &memory, const NextWord &args) {
        memory[--sp] = emu::util::byte::second_byte(pc);
        memory[--sp] = emu::util::byte::first_byte(pc);
        pc = emu::util::byte::to_uint16_t(args.sarg, args.farg);
    }

    void execute_call(std::uint16_t &pc, std::uint16_t &sp, emu::cpu8080::EmulatorMemory &memory, std::uint8_t farg, std::uint8_t sarg) {
        memory[--sp] = emu::util::byte::second_byte(pc);
        memory[--sp] = emu::util::byte::first_byte(pc);
        pc = emu::util::byte::to_uint16_t(sarg, farg);
    }

    void execute_return(std::uint16_t &pc, std::uint16_t &sp, const emu::cpu8080::EmulatorMemory &memory) {
        const std::uint8_t sarg = memory[sp++];
        const std::uint8_t farg = memory[sp++];
        pc = emu::util::byte::to_uint16_t(farg, sarg);
    }
}
