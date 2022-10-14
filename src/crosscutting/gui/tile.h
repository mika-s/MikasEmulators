#ifndef MIKA_EMULATORS_CROSSCUTTING_GUI_TILE_H
#define MIKA_EMULATORS_CROSSCUTTING_GUI_TILE_H

#include <cstddef>
#include <unordered_map>
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

        void map_to_framebuffer(
                Framebuffer &framebuffer,
                unsigned int origin_row,
                unsigned int origin_col
        );

        void map_debug_overlay_to_framebuffer(
                Framebuffer &framebuffer,
                unsigned int origin_row,
                unsigned int origin_col,
                unsigned int tile_idx
        );

        std::size_t size();

    private:
        std::vector<std::vector<Color>> m_values;
        std::size_t m_height;
        std::size_t m_width;

        Color m_white = Color::white();
        Color m_red = Color::red();

        Color get(std::size_t row, std::size_t col);


        static const std::vector<std::pair<int, int>> number_to_pixels(
                unsigned int number,
                unsigned int row_offset,
                unsigned int col_offset
        );

        static inline unsigned int offset_row_high_number = 0;
        static inline unsigned int offset_col_high_number = 1;
        static inline std::unordered_map<int, std::vector<std::pair<int, int>>> high_number_to_pixel_cache{
                {0x0, number_to_pixels(0x0, offset_row_high_number, offset_col_high_number)},
                {0x1, number_to_pixels(0x1, offset_row_high_number, offset_col_high_number)},
                {0x2, number_to_pixels(0x2, offset_row_high_number, offset_col_high_number)},
                {0x3, number_to_pixels(0x3, offset_row_high_number, offset_col_high_number)},
                {0x4, number_to_pixels(0x4, offset_row_high_number, offset_col_high_number)},
                {0x5, number_to_pixels(0x5, offset_row_high_number, offset_col_high_number)},
                {0x6, number_to_pixels(0x6, offset_row_high_number, offset_col_high_number)},
                {0x7, number_to_pixels(0x7, offset_row_high_number, offset_col_high_number)},
                {0x8, number_to_pixels(0x8, offset_row_high_number, offset_col_high_number)},
                {0x9, number_to_pixels(0x9, offset_row_high_number, offset_col_high_number)},
                {0xa, number_to_pixels(0xa, offset_row_high_number, offset_col_high_number)},
                {0xb, number_to_pixels(0xb, offset_row_high_number, offset_col_high_number)},
                {0xc, number_to_pixels(0xc, offset_row_high_number, offset_col_high_number)},
                {0xd, number_to_pixels(0xd, offset_row_high_number, offset_col_high_number)},
                {0xe, number_to_pixels(0xe, offset_row_high_number, offset_col_high_number)},
                {0xf, number_to_pixels(0xf, offset_row_high_number, offset_col_high_number)}
        };

        static inline unsigned int offset_row_low_number = 0;
        static inline unsigned int offset_col_low_number = 4;
        static inline std::unordered_map<int, std::vector<std::pair<int, int>>> low_number_to_pixel_cache{
                {0x0, number_to_pixels(0x0, offset_row_low_number, offset_col_low_number)},
                {0x1, number_to_pixels(0x1, offset_row_low_number, offset_col_low_number)},
                {0x2, number_to_pixels(0x2, offset_row_low_number, offset_col_low_number)},
                {0x3, number_to_pixels(0x3, offset_row_low_number, offset_col_low_number)},
                {0x4, number_to_pixels(0x4, offset_row_low_number, offset_col_low_number)},
                {0x5, number_to_pixels(0x5, offset_row_low_number, offset_col_low_number)},
                {0x6, number_to_pixels(0x6, offset_row_low_number, offset_col_low_number)},
                {0x7, number_to_pixels(0x7, offset_row_low_number, offset_col_low_number)},
                {0x8, number_to_pixels(0x8, offset_row_low_number, offset_col_low_number)},
                {0x9, number_to_pixels(0x9, offset_row_low_number, offset_col_low_number)},
                {0xa, number_to_pixels(0xa, offset_row_low_number, offset_col_low_number)},
                {0xb, number_to_pixels(0xb, offset_row_low_number, offset_col_low_number)},
                {0xc, number_to_pixels(0xc, offset_row_low_number, offset_col_low_number)},
                {0xd, number_to_pixels(0xd, offset_row_low_number, offset_col_low_number)},
                {0xe, number_to_pixels(0xe, offset_row_low_number, offset_col_low_number)},
                {0xf, number_to_pixels(0xf, offset_row_low_number, offset_col_low_number)}
        };
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
