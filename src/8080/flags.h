#ifndef MIKA_EMULATORS_8080_FLAGS_H
#define MIKA_EMULATORS_8080_FLAGS_H

#include "crosscutting/typedefs.h"

namespace emu::i8080 {

    class Flags {
    public:
        Flags();

        void reset();

        [[nodiscard]] u8 to_u8() const;

        void from_u8(u8 value);

        void handle_zero_flag(u8 number);

        void handle_carry_flag(u8 previous, int value_to_add);

        void handle_carry_flag_dad(u16 previous, u16 value_to_add);

        void handle_borrow_flag(u8 previous, int value_to_subtract);

        void handle_aux_carry_flag(u8 previous, u8 value_to_add, bool cf);

        void handle_aux_borrow_flag(u8 previous, u8 value_to_subtract, bool cf);

        void handle_parity_flag(u8 number);

        void handle_sign_flag(u8 number);

        static bool should_parity_flag_be_set(u8 number);

        void set_zero_flag();

        void clear_zero_flag();

        [[nodiscard]] bool is_zero_flag_set() const;

        void set_carry_flag();

        void clear_carry_flag();

        [[nodiscard]] bool is_carry_flag_set() const;

        void toggle_carry_flag();

        void set_aux_carry_flag();

        void clear_aux_carry_flag();

        [[nodiscard]] bool is_aux_carry_flag_set() const;

        void set_sign_flag();

        void clear_sign_flag();

        [[nodiscard]] bool is_sign_flag_set() const;

        void set_parity_flag();

        void clear_parity_flag();

        [[nodiscard]] bool is_parity_flag_set() const;

    private:
        bool m_carry;
        bool m_parity; // True if even number of one-bits.
        bool m_auxiliary_carry;
        bool m_zero;
        bool m_sign;
    };
}

#endif //MIKA_EMULATORS_8080_FLAGS_H
