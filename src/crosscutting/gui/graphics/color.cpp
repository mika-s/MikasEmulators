#include "color.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"

namespace emu::gui {

Color::Color(u8 alpha, u8 red, u8 green, u8 blue)
    : m_alpha(alpha)
    , m_red(red)
    , m_green(green)
    , m_blue(blue)
{
}

u32 Color::to_u32() const
{
    return emu::util::byte::to_u32(m_alpha, m_blue, m_green, m_red);
}

bool Color::is_transparent() const
{
    return m_alpha == 0;
}

Color Color::black()
{
    return { 0xff, 0, 0, 0 };
}

Color Color::transparent()
{
    return { 0, 0, 0, 0 };
}

Color Color::white()
{
    return { 0xff, 0xff, 0xff, 0xff };
}

Color Color::red()
{
    return { 0xff, 0xff, 0, 0 };
}

Color Color::green()
{
    return { 0xff, 0, 0xff, 0 };
}

Color Color::blue()
{
    return { 0xff, 0, 0, 0xff };
}

Color Color::yellow()
{
    return { 0xff, 0xff, 0xff, 0 };
}
}
