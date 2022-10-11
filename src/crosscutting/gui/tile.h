#ifndef MIKA_EMULATORS_CROSSCUTTING_GUI_TILE_H
#define MIKA_EMULATORS_CROSSCUTTING_GUI_TILE_H

#include <cstddef>
#include <vector>
#include "color.h"
#include "framebuffer.h"
#include "crosscutting/typedefs.h"

namespace emu::gui {

    class Tile {
    public:
        Tile(std::size_t height, std::size_t width);

        virtual ~Tile() = default;

        virtual bool is_initialized() {
            return true;
        }

        void set(std::size_t row, std::size_t col, Color value);

        void map_to_framebuffer(Framebuffer &framebuffer, unsigned int origin_row, unsigned int origin_col);

        std::size_t size();

    private:
        std::vector<std::vector<Color>> m_values;
        std::size_t m_height;
        std::size_t m_width;

        Color get(std::size_t row, std::size_t col);
    };

    class UninitializedTile : public Tile {
    public:
        UninitializedTile();

        bool is_initialized() override {
            return false;
        }
    };
}


#endif //MIKA_EMULATORS_CROSSCUTTING_GUI_TILE_H
