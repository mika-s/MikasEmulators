#include <fmt/format.h>
#include <stdexcept>
#include "tile.h"

namespace emu::gui {

    Tile::Tile(std::size_t height, std::size_t width)
            : m_height(height),
              m_width(width) {
        if (height != width) {
            throw std::invalid_argument("Non-square tiles not supported");
        }

        for (unsigned int row = 0; row < height; ++row) {
            m_values.emplace_back(width, Color::black());
        }
    }

    void Tile::set(std::size_t row, std::size_t col, Color value) {
        if (row > m_height - 1) {
            throw std::runtime_error(fmt::format("row of {} is too large, height is {}", row, m_height));
        } else if (col > m_width - 1) {
            throw std::runtime_error(fmt::format("col of {} is too large, width is {}", col, m_width));
        }

        m_values[row][col] = value;
    }

    Color Tile::get(std::size_t row, std::size_t col) {
        return m_values[row][col];
    }

    void Tile::map_to_framebuffer(
            Framebuffer &framebuffer,
            unsigned int origin_row,
            unsigned int origin_col
    ) {
        for (std::size_t px_row = 0; px_row < m_height; ++px_row) {
            for (std::size_t px_col = 0; px_col < m_width; ++px_col) {
                framebuffer.set(origin_row + px_row, origin_col + px_col, get(px_row, px_col));
            }
        }
    }

    std::size_t Tile::size() {
        return m_width;
    }

    void Tile::map_debug_overlay_to_framebuffer(
            Framebuffer &framebuffer,
            unsigned int origin_row,
            unsigned int origin_col,
            unsigned int tile_idx
    ) {
        const unsigned int high_digit = tile_idx >> 4;
        for (auto &pixel: high_number_to_pixel_cache[high_digit]) {
            framebuffer.set(origin_row + pixel.first, origin_col + pixel.second, m_white);
        }

        const unsigned int low_digit = tile_idx & 0x0f;
        for (auto &pixel: low_number_to_pixel_cache[low_digit]) {
            framebuffer.set(origin_row + pixel.first, origin_col + pixel.second, m_red);
        }
    }

    const std::vector<std::pair<int, int>> Tile::number_to_pixels(
            unsigned int number,
            unsigned int row_offset,
            unsigned int col_offset
    ) {
        switch (number) {
            case 0x0:
                return {
                        {row_offset + 0, col_offset + 0},
                        {row_offset + 0, col_offset + 1},
                        {row_offset + 0, col_offset + 2},
                        {row_offset + 1, col_offset + 0},
                        {row_offset + 1, col_offset + 2},
                        {row_offset + 2, col_offset + 0},
                        {row_offset + 2, col_offset + 2},
                        {row_offset + 3, col_offset + 0},
                        {row_offset + 3, col_offset + 2},
                        {row_offset + 4, col_offset + 0},
                        {row_offset + 4, col_offset + 1},
                        {row_offset + 4, col_offset + 2}
                };
            case 0x1:
                return {
                        {row_offset + 0, col_offset + 2},
                        {row_offset + 1, col_offset + 2},
                        {row_offset + 2, col_offset + 2},
                        {row_offset + 3, col_offset + 2},
                        {row_offset + 4, col_offset + 2}
                };
            case 0x2:
                return {
                        {row_offset + 0, col_offset + 0},
                        {row_offset + 0, col_offset + 1},
                        {row_offset + 0, col_offset + 2},
                        {row_offset + 1, col_offset + 2},
                        {row_offset + 2, col_offset + 0},
                        {row_offset + 2, col_offset + 1},
                        {row_offset + 2, col_offset + 2},
                        {row_offset + 3, col_offset + 0},
                        {row_offset + 4, col_offset + 0},
                        {row_offset + 4, col_offset + 1},
                        {row_offset + 4, col_offset + 2}
                };
            case 0x3:
                return {
                        {row_offset + 0, col_offset + 0},
                        {row_offset + 0, col_offset + 1},
                        {row_offset + 0, col_offset + 2},
                        {row_offset + 1, col_offset + 2},
                        {row_offset + 2, col_offset + 1},
                        {row_offset + 2, col_offset + 2},
                        {row_offset + 3, col_offset + 2},
                        {row_offset + 4, col_offset + 0},
                        {row_offset + 4, col_offset + 1},
                        {row_offset + 4, col_offset + 2}
                };
            case 0x4:
                return {
                        {row_offset + 0, col_offset + 0},
                        {row_offset + 0, col_offset + 2},
                        {row_offset + 1, col_offset + 0},
                        {row_offset + 1, col_offset + 2},
                        {row_offset + 2, col_offset + 0},
                        {row_offset + 2, col_offset + 1},
                        {row_offset + 2, col_offset + 2},
                        {row_offset + 3, col_offset + 2},
                        {row_offset + 4, col_offset + 2}
                };
            case 0x5:
                return {
                        {row_offset + 0, col_offset + 0},
                        {row_offset + 0, col_offset + 1},
                        {row_offset + 0, col_offset + 2},
                        {row_offset + 1, col_offset + 0},
                        {row_offset + 2, col_offset + 0},
                        {row_offset + 2, col_offset + 1},
                        {row_offset + 2, col_offset + 2},
                        {row_offset + 3, col_offset + 2},
                        {row_offset + 4, col_offset + 0},
                        {row_offset + 4, col_offset + 1},
                        {row_offset + 4, col_offset + 2}
                };
            case 0x6:
                return {
                        {row_offset + 0, col_offset + 0},
                        {row_offset + 0, col_offset + 1},
                        {row_offset + 0, col_offset + 2},
                        {row_offset + 1, col_offset + 2},
                        {row_offset + 2, col_offset + 0},
                        {row_offset + 2, col_offset + 2},
                        {row_offset + 3, col_offset + 0},
                        {row_offset + 3, col_offset + 2},
                        {row_offset + 4, col_offset + 0},
                        {row_offset + 4, col_offset + 1},
                        {row_offset + 4, col_offset + 2}
                };
            case 0x7:
                return {
                        {row_offset + 0, col_offset + 0},
                        {row_offset + 0, col_offset + 1},
                        {row_offset + 0, col_offset + 2},
                        {row_offset + 1, col_offset + 2},
                        {row_offset + 2, col_offset + 2},
                        {row_offset + 3, col_offset + 2},
                        {row_offset + 4, col_offset + 2}
                };
            case 0x8:
                return {
                        {row_offset + 0, col_offset + 0},
                        {row_offset + 0, col_offset + 1},
                        {row_offset + 0, col_offset + 2},
                        {row_offset + 1, col_offset + 0},
                        {row_offset + 1, col_offset + 2},
                        {row_offset + 2, col_offset + 0},
                        {row_offset + 2, col_offset + 1},
                        {row_offset + 2, col_offset + 2},
                        {row_offset + 3, col_offset + 0},
                        {row_offset + 3, col_offset + 2},
                        {row_offset + 4, col_offset + 0},
                        {row_offset + 4, col_offset + 1},
                        {row_offset + 4, col_offset + 2}
                };
            case 0x9:
                return {
                        {row_offset + 0, col_offset + 0},
                        {row_offset + 0, col_offset + 1},
                        {row_offset + 0, col_offset + 2},
                        {row_offset + 1, col_offset + 0},
                        {row_offset + 1, col_offset + 2},
                        {row_offset + 2, col_offset + 0},
                        {row_offset + 2, col_offset + 1},
                        {row_offset + 2, col_offset + 2},
                        {row_offset + 3, col_offset + 2},
                        {row_offset + 4, col_offset + 2}
                };
            case 0xa:
                return {
                        {row_offset + 0, col_offset + 0},
                        {row_offset + 0, col_offset + 1},
                        {row_offset + 0, col_offset + 2},
                        {row_offset + 1, col_offset + 0},
                        {row_offset + 1, col_offset + 2},
                        {row_offset + 2, col_offset + 0},
                        {row_offset + 2, col_offset + 2},
                        {row_offset + 3, col_offset + 0},
                        {row_offset + 3, col_offset + 2},
                        {row_offset + 4, col_offset + 0},
                        {row_offset + 4, col_offset + 2}
                };
            case 0xb:
                return {
                        {row_offset + 0, col_offset + 0},
                        {row_offset + 0, col_offset + 1},
                        {row_offset + 1, col_offset + 0},
                        {row_offset + 1, col_offset + 2},
                        {row_offset + 2, col_offset + 0},
                        {row_offset + 2, col_offset + 1},
                        {row_offset + 2, col_offset + 2},
                        {row_offset + 3, col_offset + 0},
                        {row_offset + 3, col_offset + 2},
                        {row_offset + 4, col_offset + 0},
                        {row_offset + 4, col_offset + 1}
                };
            case 0xc:
                return {
                        {row_offset + 0, col_offset + 0},
                        {row_offset + 0, col_offset + 1},
                        {row_offset + 0, col_offset + 2},
                        {row_offset + 1, col_offset + 0},
                        {row_offset + 2, col_offset + 0},
                        {row_offset + 3, col_offset + 0},
                        {row_offset + 4, col_offset + 0},
                        {row_offset + 4, col_offset + 1},
                        {row_offset + 4, col_offset + 2}
                };
            case 0xd:
                return {
                        {row_offset + 0, col_offset + 0},
                        {row_offset + 0, col_offset + 1},
                        {row_offset + 1, col_offset + 0},
                        {row_offset + 1, col_offset + 2},
                        {row_offset + 2, col_offset + 0},
                        {row_offset + 2, col_offset + 2},
                        {row_offset + 3, col_offset + 0},
                        {row_offset + 3, col_offset + 2},
                        {row_offset + 4, col_offset + 0},
                        {row_offset + 4, col_offset + 1}
                };
            case 0xe:
                return {
                        {row_offset + 0, col_offset + 0},
                        {row_offset + 0, col_offset + 1},
                        {row_offset + 0, col_offset + 2},
                        {row_offset + 1, col_offset + 0},
                        {row_offset + 2, col_offset + 0},
                        {row_offset + 2, col_offset + 1},
                        {row_offset + 2, col_offset + 2},
                        {row_offset + 3, col_offset + 0},
                        {row_offset + 4, col_offset + 0},
                        {row_offset + 4, col_offset + 1},
                        {row_offset + 4, col_offset + 2}
                };
            case 0xf:
                return {
                        {row_offset + 0, col_offset + 0},
                        {row_offset + 0, col_offset + 1},
                        {row_offset + 0, col_offset + 2},
                        {row_offset + 1, col_offset + 0},
                        {row_offset + 2, col_offset + 0},
                        {row_offset + 2, col_offset + 1},
                        {row_offset + 2, col_offset + 2},
                        {row_offset + 3, col_offset + 0},
                        {row_offset + 4, col_offset + 0}
                };
            default:
                throw std::invalid_argument("Number cannot be larger than 0xf");
        }
    }

    UninitializedTile::UninitializedTile() : Tile(0, 0) {}
}
