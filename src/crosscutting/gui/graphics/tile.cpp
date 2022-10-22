#include <fmt/format.h>
#include <stdexcept>
#include "tile.h"

namespace emu::gui {

    Tile::Tile(std::size_t height, std::size_t width)
            : m_height(height),
              m_width(width) {
        if (height != width) {
            throw std::invalid_argument(fmt::format("Non-square tiles not supported: {}x{}", height, width));
        } else if (m_height > UINT32_MAX) {
            throw std::invalid_argument(fmt::format("Tile height too large: {}", height));
        } else if (m_width > UINT32_MAX) {
            throw std::invalid_argument(fmt::format("Tile width too large: {}", width));
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
        for (unsigned int px_row = 0; px_row < static_cast<unsigned int>(m_height); ++px_row) {
            for (unsigned int px_col = 0; px_col < static_cast<unsigned int>(m_width); ++px_col) {

                framebuffer.set(origin_row + px_row, origin_col + px_col, get(px_row, px_col));
            }
        }
    }

    std::size_t Tile::size() {
        return m_width;
    }

    UninitializedTile::UninitializedTile() : Tile(0, 0) {}
}
