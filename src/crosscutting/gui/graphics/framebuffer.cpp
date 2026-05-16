#include "framebuffer.h"
#include "crosscutting/typedefs.h"
#include "gui/graphics/color.h"
#include <algorithm>
#include <format>
#include <iterator>
#include <stdexcept>

namespace emu::gui {

Framebuffer::Framebuffer(const unsigned int height, const unsigned int width, const Color init_color)
    : m_height(height)
    , m_width(width)
{
    for (unsigned int row = 0; row < height; ++row) {
        std::vector<u32> new_row;
        for (unsigned int col = 0; col < width; ++col) {
            new_row.emplace_back(init_color.to_u32());
        }
        m_values.push_back(new_row);
    }
}

void Framebuffer::set(unsigned int row, unsigned int col, const Color value)
{
    if (row > m_height - 1) {
        throw std::runtime_error(std::format("row of {} is too large, height is {}", row, m_height));
    }
    if (col > m_width - 1) {
        throw std::runtime_error(std::format("col of {} is too large, width is {}", col, m_width));
    }

    m_values[row][col] = value.to_u32();
}

void Framebuffer::flip_vertical()
{
    std::reverse(std::begin(m_values), std::end(m_values));
}

void Framebuffer::clear()
{
    const u32 black = Color::black().to_u32();

    for (unsigned int row = 0; row < m_height; ++row) {
        for (unsigned int col = 0; col < m_width; ++col) {
            m_values[row][col] = black;
        }
    }
}

auto Framebuffer::to_output_vector() const -> std::vector<u32>
{
    std::vector<u32> output;

    for (unsigned int row = 0; row < m_height; ++row) {
        for (unsigned int col = 0; col < m_width; ++col) {
            output.push_back(m_values[row][col]);
        }
    }

    return output;
}

auto Framebuffer::height() const -> unsigned int
{
    return m_height;
}

auto Framebuffer::width() const -> unsigned int
{
    return m_width;
}
}
