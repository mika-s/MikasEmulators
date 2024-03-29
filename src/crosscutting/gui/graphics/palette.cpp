#include "palette.h"
#include "gui/graphics/color.h"
#include <fmt/core.h>
#include <stdexcept>

namespace emu::gui {

Palette::Palette(Color color1, Color color2, Color color3, Color color4)
    : m_color1(color1)
    , m_color2(color2)
    , m_color3(color3)
    , m_color4(color4)
{
}

Color const& Palette::operator[](std::size_t address) const
{
    if (address == 0) {
        return m_color1;
    } else if (address == 1) {
        return m_color2;
    } else if (address == 2) {
        return m_color3;
    } else if (address == 3) {
        return m_color4;
    } else {
        throw std::runtime_error(fmt::format("Invalid color index in Palette: {}", address));
    }
}
}
