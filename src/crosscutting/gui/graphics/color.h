#pragma once

#include "crosscutting/typedefs.h"

namespace emu::gui {
class Color {
public:
    Color(u8 alpha, u8 red, u8 green, u8 blue);

    [[nodiscard]] auto is_transparent() const -> bool;

    [[nodiscard]] auto to_u32() const -> u32;

    static auto black() -> Color;

    static auto transparent() -> Color;

    static auto white() -> Color;

    static auto red() -> Color;

    static auto green() -> Color;

    static auto blue() -> Color;

    static auto yellow() -> Color;

private:
    u8 m_alpha;
    u8 m_red;
    u8 m_green;
    u8 m_blue;
};
}
