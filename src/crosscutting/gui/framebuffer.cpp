#include <fmt/format.h>
#include <stdexcept>
#include "framebuffer.h"

namespace emu::gui {

    Framebuffer::Framebuffer(unsigned int height, unsigned int width, Color init_color)
            : m_height(height),
              m_width(width) {
        for (unsigned int row = 0; row < height; ++row) {
            std::vector<u32> new_row;
            for (unsigned int col = 0; col < width; ++col) {
                new_row.emplace_back(init_color.to_u32());
            }
            m_values.push_back(new_row);
        }
    }

    void Framebuffer::set(unsigned int row, unsigned int col, Color value) {
        if (row > m_height - 1) {
            throw std::runtime_error(fmt::format("row of {} is too large, height is {}", row, m_height));
        } else if (col > m_width - 1) {
            throw std::runtime_error(fmt::format("col of {} is too large, width is {}", col, m_width));
        }

        m_values[row][col] = value.to_u32();
    }

    std::vector<u32> Framebuffer::to_output_vector() {
        std::vector<u32> output;

        for (unsigned int row = 0; row < m_height; ++row) {
            for (unsigned int col = 0; col < m_width; ++col) {
                output.push_back(m_values[row][col]);
            }
        }

        return output;
    }

    unsigned int Framebuffer::height() {
        return m_height;
    }

    unsigned int Framebuffer::width() {
        return m_width;
    }
}