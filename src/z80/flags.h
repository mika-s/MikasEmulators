#ifndef MIKA_EMULATORS_Z80_FLAGS_H
#define MIKA_EMULATORS_Z80_FLAGS_H

#include "crosscutting/typedefs.h"

namespace emu::z80 {

    class Flags {
    public:
        Flags();

        void reset();

        [[nodiscard]] u8 to_u8() const;

        void from_u8(u8 value);

        void handle_zero_flag(u8 number);

        void handle_zero_flag(u16 number);

        void handle_carry_flag(u8 previous, int to_add, bool cf);

        void handle_carry_flag(u16 previous, u16 to_add);

        void handle_borrow_flag(u8 previous, int to_subtract, bool cf);

        void handle_half_carry_flag(u8 previous, u8 to_add, bool cf);

        void handle_half_carry_flag(u16 previous, u16 to_add, bool cf);

        void handle_half_borrow_flag(u8 previous, u8 to_subtract, bool cf);

        void handle_borrow_flag(u16 previous, int to_subtract, bool cf);

        void handle_parity_flag(u8 number);

        void handle_overflow_flag(u8 previous, u8 to_add, bool cf);

        void handle_sign_flag(u8 number);

        void handle_sign_flag(u16 number);

        static bool should_parity_flag_be_set(u8 number);

        static bool should_overflow_flag_be_set(u8 previous, u8 to_add, bool cf);

        void set_zero_flag();

        void clear_zero_flag();

        [[nodiscard]] bool is_zero_flag_set() const;

        void set_carry_flag();

        void clear_carry_flag();

        [[nodiscard]] bool is_carry_flag_set() const;

        void toggle_carry_flag();

        void set_half_carry_flag();

        void clear_half_carry_flag();

        [[nodiscard]] bool is_half_carry_flag_set() const;

        void set_add_subtract_flag();

        void clear_add_subtract_flag();

        [[nodiscard]] bool is_add_subtract_flag_set() const;

        void set_sign_flag();

        void clear_sign_flag();

        [[nodiscard]] bool is_sign_flag_set() const;

        void set_parity_overflow_flag();

        void clear_parity_overflow_flag();

        [[nodiscard]] bool is_parity_overflow_flag_set() const;

    private:
        static constexpr unsigned int msb_u16 = 15;
        static constexpr unsigned int msb = 7;
        static constexpr unsigned int msb_first_nibble = 3;
        static constexpr unsigned int msb_first_nibble_u16 = 11;

        bool m_carry;
        bool m_add_subtract;
        bool m_parity_overflow; // Parity: True if even number of one-bits.
        bool m_half_carry;
        bool m_zero;
        bool m_sign;
    };
}

#endif //MIKA_EMULATORS_Z80_FLAGS_H
