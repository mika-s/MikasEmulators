#ifndef MIKA_EMULATORS_8080_FLAGS_H
#define MIKA_EMULATORS_8080_FLAGS_H

namespace emu::cpu8080 {

    class Flags {
    public:
        Flags();

        void reset();

        [[nodiscard]] std::uint8_t to_uint8_t() const;

        void from_uint8_t(std::uint8_t value);

        void handle_zero_flag(std::uint8_t number);

        void handle_carry_flag(std::uint8_t previous, int value_to_add);

        void handle_carry_flag_dad(std::uint16_t previous, std::uint16_t value_to_add);

        void handle_borrow_flag(std::uint8_t previous, int value_to_subtract);

        void handle_aux_carry_flag(std::uint8_t previous, std::uint8_t value_to_add, bool cf);

        void handle_aux_borrow_flag(std::uint8_t previous, std::uint8_t value_to_subtract, bool cf);

        void handle_parity_flag(std::uint8_t number);

        void handle_sign_flag(std::uint8_t number);

        static bool should_parity_flag_be_set(std::uint8_t number);

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
        bool carry;
        bool parity; // True if even number of one-bits.
        bool auxiliary_carry;
        bool zero;
        bool sign;
    };
}

#endif //MIKA_EMULATORS_8080_FLAGS_H
