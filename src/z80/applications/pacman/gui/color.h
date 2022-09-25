#ifndef MIKA_EMULATORS_Z80_APPLICATIONS_PACMAN_GUI_COLOR_H
#define MIKA_EMULATORS_Z80_APPLICATIONS_PACMAN_GUI_COLOR_H

#include "crosscutting/typedefs.h"

namespace emu::z80::applications::pacman {
    class Color {
    public:
        Color(u8 alpha, u8 red, u8 green, u8 blue);

        static Color black();

        [[nodiscard]] bool is_transparent() const;

        u32 to_u32();

    private:
        u8 m_alpha;
        u8 m_red;
        u8 m_green;
        u8 m_blue;
    };
}

#endif //MIKA_EMULATORS_Z80_APPLICATIONS_PACMAN_GUI_COLOR_H
