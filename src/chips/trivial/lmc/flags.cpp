#include "flags.h"
#include "chips/trivial/lmc/usings.h"
#include "crosscutting/misc/uinteger.h"
#include "crosscutting/util/byte_util.h"

namespace emu::lmc {

    using emu::util::byte::borrow_from;
    using emu::util::byte::carried_out_of;
    using emu::util::byte::is_bit_set;

    Flags::Flags()
        : m_negative(false) {}

    void Flags::reset() {
        clear_negative_flag();
    }

    void Flags::handle_negative_flag(Data previous, Data value) {
        if (previous < value) {
            set_negative_flag();
        } else {
            clear_negative_flag();
        }
    }

    void Flags::set_negative_flag() {
        m_negative = true;
    }

    void Flags::clear_negative_flag() {
        m_negative = false;
    }

    bool Flags::is_negative_flag_set() const {
        return m_negative;
    }
}
