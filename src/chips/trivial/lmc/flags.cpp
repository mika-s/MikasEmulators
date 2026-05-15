#include "flags.h"
#include "chips/trivial/lmc/usings.h"
#include "crosscutting/misc/uinteger.h"
#include "crosscutting/util/byte_util.h"

namespace emu::lmc {

using emu::util::byte::borrow_from;
using emu::util::byte::carried_out_of;
using emu::util::byte::is_bit_set;

Flags::Flags()
    : m_negative(false)
{
}

auto Flags::to_u8() const -> u8
{
    return m_negative ? 1 : 0;
}

void Flags::reset()
{
    clear_negative_flag();
}

void Flags::handle_negative_flag(const Data previous, const Data value)
{
    if (previous < value) {
        set_negative_flag();
    } else {
        clear_negative_flag();
    }
}

void Flags::set_negative_flag()
{
    m_negative = true;
}

void Flags::clear_negative_flag()
{
    m_negative = false;
}

auto Flags::is_negative_flag_set() const -> bool
{
    return m_negative;
}
}
