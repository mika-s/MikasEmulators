#pragma once

#include "chips/trivial/lmc/usings.h"
#include "crosscutting/typedefs.h"

namespace emu::lmc {

class Flags {
public:
    Flags();

    [[nodiscard]] auto to_u8() const -> u8;

    void reset();

    void handle_negative_flag(Data previous, Data value);

    [[nodiscard]] auto is_negative_flag_set() const -> bool;

private:
    bool m_negative;

    void set_negative_flag();

    void clear_negative_flag();
};
}
