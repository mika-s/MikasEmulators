#ifndef MIKA_EMULATORS_Z80_APPLICATIONS_PACMAN_GUI_TILE_H
#define MIKA_EMULATORS_Z80_APPLICATIONS_PACMAN_GUI_TILE_H

#include <cstddef>
#include <vector>
#include "crosscutting/typedefs.h"
#include "crosscutting/gui/color.h"
#include "crosscutting/gui/framebuffer.h"

namespace emu::z80::applications::pacman {

    using emu::gui::Color;
    using emu::gui::Framebuffer;

    class Tile {
    public:
        Tile(size_t height, size_t width);

        virtual ~Tile() = default;

        virtual bool is_initialized() {
            return true;
        }

        void set(size_t row, size_t col, Color value);

        void map_to_framebuffer(Framebuffer &framebuffer, unsigned int origin_row, unsigned int origin_col);

    private:
        std::vector<std::vector<Color>> m_values;
        size_t m_height;
        size_t m_width;

        Color get(size_t row, size_t col);
    };

    class UninitializedTile : public Tile {
    public:
        UninitializedTile();

        bool is_initialized() override {
            return false;
        }
    };
}


#endif //MIKA_EMULATORS_Z80_APPLICATIONS_PACMAN_GUI_TILE_H
