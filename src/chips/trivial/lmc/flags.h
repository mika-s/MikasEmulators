#pragma once

#include "chips/trivial/lmc/usings.h"
#include "crosscutting/typedefs.h"

namespace emu::lmc {

class Flags {
public:
    Flags();

    [[nodiscard]] u8 to_u8() const;

    void reset();

    void handle_negative_flag(Data previous, Data value);

    [[nodiscard]] bool is_negative_flag_set() const;

private:
    bool m_negative;

    void set_negative_flag();

    void clear_negative_flag();
};
}
