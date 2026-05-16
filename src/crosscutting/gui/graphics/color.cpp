#include "color.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"

namespace emu::gui {

Color::Color(const u8 alpha, const u8 red, const u8 green, const u8 blue)
    : m_alpha(alpha)
    , m_red(red)
    , m_green(green)
    , m_blue(blue)
{
}

auto Color::to_u32() const -> u32
{
    return emu::util::byte::to_u32(m_alpha, m_blue, m_green, m_red);
}

auto Color::is_transparent() const -> bool
{
    return m_alpha == 0;
}

auto Color::black() -> Color
{
    return { 0xff, 0, 0, 0 };
}

auto Color::transparent() -> Color
{
    return { 0, 0, 0, 0 };
}

auto Color::white() -> Color
{
    return { 0xff, 0xff, 0xff, 0xff };
}

auto Color::red() -> Color
{
    return { 0xff, 0xff, 0, 0 };
}

auto Color::green() -> Color
{
    return { 0xff, 0, 0xff, 0 };
}

auto Color::blue() -> Color
{
    return { 0xff, 0, 0, 0xff };
}

auto Color::yellow() -> Color
{
    return { 0xff, 0xff, 0xff, 0 };
}
}
