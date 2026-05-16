#include "palette.h"
#include "gui/graphics/color.h"
#include <format>
#include <stdexcept>

namespace emu::gui {

Palette::Palette(const Color color1, const Color color2, const Color color3, const Color color4)
    : m_color1(color1)
    , m_color2(color2)
    , m_color3(color3)
    , m_color4(color4)
{
}

auto Palette::operator[](std::size_t address) const -> Color const&
{
    if (address == 0) {
        return m_color1;
    }
    if (address == 1) {
        return m_color2;
    }
    if (address == 2) {
        return m_color3;
    }
    if (address == 3) {
        return m_color4;
    }

    throw std::runtime_error(std::format("Invalid color index in Palette: {}", address));
}
}
