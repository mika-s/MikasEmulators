#include "color.h"
#include "crosscutting/util/byte_util.h"

namespace emu::gui {

    Color::Color(u8 alpha, u8 red, u8 green, u8 blue)
            : m_alpha(alpha),
              m_red(red),
              m_green(green),
              m_blue(blue) {
    }

    u32 Color::to_u32() {
        return emu::util::byte::to_u32(m_alpha, m_blue, m_green, m_red);
    }

    Color Color::black() {
        return {0xff, 0, 0, 0};
    }

    bool Color::is_transparent() const {
        return m_alpha == 0;
    }
}
