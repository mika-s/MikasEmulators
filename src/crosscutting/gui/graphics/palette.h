#ifndef MIKA_EMULATORS_CROSSCUTTING_GUI_GRAPHICS_PALETTE_H
#define MIKA_EMULATORS_CROSSCUTTING_GUI_GRAPHICS_PALETTE_H

#include "color.h"
#include <cstddef>

namespace emu::gui {

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

#endif //MIKA_EMULATORS_CROSSCUTTING_GUI_GRAPHICS_PALETTE_H
