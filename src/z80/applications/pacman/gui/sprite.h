#ifndef MIKA_EMULATORS_Z80_APPLICATIONS_PACMAN_GUI_SPRITE_H
#define MIKA_EMULATORS_Z80_APPLICATIONS_PACMAN_GUI_SPRITE_H

#include <cstddef>
#include <vector>
#include "crosscutting/typedefs.h"
#include "crosscutting/gui/color.h"
#include "crosscutting/gui/framebuffer.h"

namespace emu::z80::applications::pacman {

    using emu::gui::Color;
    using emu::gui::Framebuffer;

    class Sprite {
    public:
        Sprite(size_t height, size_t width);

        virtual ~Sprite() = default;

        virtual bool is_initialized() {
            return true;
        }

        void set(size_t row, size_t col, Color value);

        void flip_horizontal();

        void flip_vertical();

        void map_to_framebuffer(Framebuffer &framebuffer, int origin_row, int origin_col);

    private:
        std::vector<std::vector<Color>> m_values;
        size_t m_height;
        size_t m_width;

        Color get(size_t row, size_t col);
    };

    class UninitializedSprite : public Sprite {
    public:
        UninitializedSprite();

        bool is_initialized() override {
            return false;
        }
    };
}

#endif //MIKA_EMULATORS_Z80_APPLICATIONS_PACMAN_GUI_SPRITE_H
