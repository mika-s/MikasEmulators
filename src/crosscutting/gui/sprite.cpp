#include <algorithm>
#include <fmt/format.h>
#include <stdexcept>
#include "sprite.h"

namespace emu::gui {

    Sprite::Sprite(size_t height, size_t width)
            : m_height(height),
              m_width(width) {
        for (unsigned int row = 0; row < height; ++row) {
            m_values.emplace_back(width, Color::black());
        }
    }

    void Sprite::set(size_t row, size_t col, Color value) {
        if (row > m_height - 1) {
            throw std::runtime_error(fmt::format("row of %d is too large, height is %d", row, m_height));
        } else if (col > m_width - 1) {
            throw std::runtime_error(fmt::format("col of %d is too large, width is %d", col, m_width));
        }

        m_values[row][col] = value;
    }

    void Sprite::flip_horizontal() {
        std::for_each(std::begin(m_values), std::end(m_values),
                      [](auto &i) { std::reverse(std::begin(i), std::end(i)); });
    }

    void Sprite::flip_vertical() {
        std::reverse(std::begin(m_values), std::end(m_values));
    }

    void Sprite::map_to_framebuffer(Framebuffer &framebuffer, int origin_row, int origin_col) {
        for (size_t row = 0; row < m_height; ++row) {
            for (size_t col = 0; col < m_width; ++col) {
                const Color pixel = get(row, col);

                if (pixel.is_transparent()) {
                    continue;
                }

                if (origin_col + col >= m_width || origin_row + row >= m_height) {
                    continue;
                }

                framebuffer.set(origin_row + row, origin_col + col, pixel);
            }
        }
    }

    Color Sprite::get(size_t row, size_t col) {
        return m_values[row][col];
    }

    UninitializedSprite::UninitializedSprite() : Sprite(0, 0) {}
}
