#ifndef MIKA_EMULATORS_Z80_APPLICATIONS_PACMAN_GUI_PALETTE_H
#define MIKA_EMULATORS_Z80_APPLICATIONS_PACMAN_GUI_PALETTE_H

#include "crosscutting/typedefs.h"
#include "crosscutting/gui/color.h"

namespace emu::z80::applications::pacman {

    using emu::gui::Color;

    class Palette {
    public:
        Palette(Color color1, Color color2, Color color3, Color color4);

        const Color &operator[](std::size_t address) const;

    private:
        Color m_color1;
        Color m_color2;
        Color m_color3;
        Color m_color4;
    };
}

#endif //MIKA_EMULATORS_Z80_APPLICATIONS_PACMAN_GUI_PALETTE_H
