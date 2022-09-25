#include <fmt/format.h>
#include <stdexcept>
#include "framebuffer.h"

namespace emu::z80::applications::pacman {

    Framebuffer::Framebuffer(unsigned int height, unsigned int width, Color init_color)
            : m_height(height),
              m_width(width) {
        for (unsigned int row = 0; row < height; ++row) {
            std::vector<Color> new_row;
            for (unsigned int col = 0; col < width; ++col) {
                new_row.emplace_back(init_color);
            }
            m_values.push_back(new_row);
        }
    }

    void Framebuffer::set(unsigned int row, unsigned int col, Color value) {
        if (row > m_height - 1) {
            throw std::runtime_error(fmt::format("row of %d is too large, height is %d", row, m_height));
        } else if (col > m_width - 1) {
            throw std::runtime_error(fmt::format("col of %d is too large, width is %d", col, m_width));
        }

        m_values[row][col] = value;
    }

    std::vector<u32> Framebuffer::to_output_vector() {
        std::vector<u32> output;

        for (unsigned int row = 0; row < m_height; ++row) {
            for (unsigned int col = 0; col < m_width; ++col) {
                Color color = m_values[row][col];
                output.push_back(color.to_u32());
            }
        }

        return output;
    }
}
