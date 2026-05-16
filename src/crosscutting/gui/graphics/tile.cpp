#include "tile.h"
#include "gui/graphics/color.h"
#include "gui/graphics/framebuffer.h"
#include <cstdint>
#include <format>
#include <stdexcept>

namespace emu::gui {

Tile::Tile(std::size_t height, std::size_t width)
    : m_height(height)
    , m_width(width)
{
    if (height != width) {
        throw std::invalid_argument(std::format("Non-square tiles not supported: {}x{}", height, width));
    }
    if (m_height > UINT32_MAX) {
        throw std::invalid_argument(std::format("Tile height too large: {}", height));
    }
    if (m_width > UINT32_MAX) {
        throw std::invalid_argument(std::format("Tile width too large: {}", width));
    }

    for (unsigned int row = 0; row < height; ++row) {
        m_values.emplace_back(width, Color::black());
    }
}

void Tile::set(std::size_t row, std::size_t col, const Color value)
{
    if (row > m_height - 1) {
        throw std::runtime_error(std::format("row of {} is too large, height is {}", row, m_height));
    }
    if (col > m_width - 1) {
        throw std::runtime_error(std::format("col of {} is too large, width is {}", col, m_width));
    }

    m_values[row][col] = value;
}

auto Tile::get(const std::size_t row, const std::size_t col) const -> Color
{
    return m_values[row][col];
}

void Tile::map_to_framebuffer(
    Framebuffer& framebuffer,
    const unsigned int origin_row,
    const unsigned int origin_col) const
{
    for (unsigned int px_row = 0; px_row < static_cast<unsigned int>(m_height); ++px_row) {
        for (unsigned int px_col = 0; px_col < static_cast<unsigned int>(m_width); ++px_col) {

            framebuffer.set(origin_row + px_row, origin_col + px_col, get(px_row, px_col));
        }
    }
}

auto Tile::size() const -> std::size_t
{
    return m_width;
}

UninitializedTile::UninitializedTile()
    : Tile(0, 0)
{
}
}
