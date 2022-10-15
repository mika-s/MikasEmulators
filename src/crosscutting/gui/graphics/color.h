#ifndef MIKA_EMULATORS_CROSSCUTTING_GUI_GRAPHICS_COLOR_H
#define MIKA_EMULATORS_CROSSCUTTING_GUI_GRAPHICS_COLOR_H

#include "crosscutting/typedefs.h"

namespace emu::gui {
    class Color {
    public:
        Color(u8 alpha, u8 red, u8 green, u8 blue);

        [[nodiscard]] bool is_transparent() const;

        u32 to_u32();

        static Color black();

        static Color white();

        static Color red();

        static Color green();

        static Color blue();

        static Color yellow();

    private:
        u8 m_alpha;
        u8 m_red;
        u8 m_green;
        u8 m_blue;
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_GUI_GRAPHICS_COLOR_H
