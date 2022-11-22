#ifndef MIKA_EMULATORS_CHIPS_LMC_FLAGS_H
#define MIKA_EMULATORS_CHIPS_LMC_FLAGS_H

#include "crosscutting/typedefs.h"

namespace emu::lmc {

    class Flags {
    public:
        Flags();

        void reset();

        void handle_negative_flag(u16 previous, u16 value);

        [[nodiscard]] bool is_negative_flag_set() const;

    private:
        static constexpr u16 max_value = 100;

        bool m_negative;

        void set_negative_flag();

        void clear_negative_flag();
    };
}

#endif //MIKA_EMULATORS_CHIPS_LMC_FLAGS_H
