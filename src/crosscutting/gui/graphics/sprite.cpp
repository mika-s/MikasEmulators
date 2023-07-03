#include "sprite.h"
#include "gui/graphics/color.h"
#include "gui/graphics/framebuffer.h"
#include <algorithm>
#include <cstdint>
#include <fmt/core.h>
#include <iterator>
#include <stdexcept>

namespace emu::gui {

Sprite::Sprite(std::size_t height, std::size_t width)
    : m_height(height)
    , m_width(width)
{
    if (height != width) {
        throw std::invalid_argument("Non-square tiles not supported");
    } else if (m_height > UINT32_MAX) {
        throw std::invalid_argument(fmt::format("Sprite height too large: {}", height));
    } else if (m_width > UINT32_MAX) {
        throw std::invalid_argument(fmt::format("Sprite width too large: {}", width));
    }

    for (unsigned int row = 0; row < height; ++row) {
        m_values.emplace_back(width, Color::transparent());
    }
}

void Sprite::set(std::size_t row, std::size_t col, Color value)
{
    if (row > m_height - 1) {
        throw std::runtime_error(fmt::format("row of {} is too large, height is {}", row, m_height));
    } else if (col > m_width - 1) {
        throw std::runtime_error(fmt::format("col of {} is too large, width is {}", col, m_width));
    }

    m_values[row][col] = value;
}

void Sprite::flip_horizontal()
{
    std::for_each(
        std::begin(m_values),
        std::end(m_values),
        [](auto& i) { std::reverse(std::begin(i), std::end(i)); });
}

void Sprite::flip_vertical()
{
    std::reverse(std::begin(m_values), std::end(m_values));
}

void Sprite::map_to_framebuffer(Framebuffer& framebuffer, int origin_row, int origin_col)
{
    for (unsigned int row = 0; row < m_height; ++row) {
        for (unsigned int col = 0; col < m_width; ++col) {
            const Color pixel = get(row, col);

            if (pixel.is_transparent()) {
                continue;
            }

            if (origin_col + col >= framebuffer.width() || origin_row + row >= framebuffer.height()) {
                continue;
            }

            framebuffer.set(origin_row + row, origin_col + col, pixel);
        }
    }
}

Color Sprite::get(std::size_t row, std::size_t col)
{
    return m_values[row][col];
}

std::size_t Sprite::size() const
{
    return m_width;
}

UninitializedSprite::UninitializedSprite()
    : Sprite(0, 0)
{
}
}
