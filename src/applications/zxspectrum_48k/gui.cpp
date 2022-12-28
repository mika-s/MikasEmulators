#include "gui.h"
#include "crosscutting/util/byte_util.h"
#include <stdexcept>

namespace emu::applications::zxspectrum_48k {

using emu::util::byte::is_bit_set;

void Gui::create_table()
{
    m_lookup_table.reserve(256);

    m_has_created_table = true;
}

void Gui::draw_borders(Framebuffer& framebuffer, u8 border_color)
{
    const Color color = s_ordinary_colors[border_color];

    for (int row = 0; row < s_height; row++) {
        for (int col = 0; col < s_width_both_borders / 2; col++) {
            framebuffer.set(row, col, color);
        }
    }

    for (int row = 0; row < s_height; row++) {
        for (int col = s_width - s_width_both_borders / 2; col < s_width; col++) {
            framebuffer.set(row, col, color);
        }
    }

    for (int row = 0; row < s_height_both_borders / 2; row++) {
        for (int col = 0; col < s_width; col++) {
            framebuffer.set(row, col, color);
        }
    }

    for (int row = s_height - s_height_both_borders / 2; row < s_height; row++) {
        for (int col = 0; col < s_width; col++) {
            framebuffer.set(row, col, color);
        }
    }
}

Color Gui::find_ink(u8 value, bool is_bright)
{
    const u8 number = value & s_ink_mask;
    return is_bright ? s_bright_colors[number] : s_ordinary_colors[number];
}

Color Gui::find_paper(u8 value, bool is_bright)
{
    const u8 number = (value & s_paper_mask) >> s_paper_shift;
    return is_bright ? s_bright_colors[number] : s_ordinary_colors[number];
}

bool Gui::find_flash_mode(u8 value)
{
    return is_bit_set(value, s_flash_bit);
}

bool Gui::find_bright_mode(u8 value)
{
    return is_bit_set(value, s_bright_bit);
}

u16 Gui::display_address_from_xy(u8 row, u8 col, u8 pixel_line)
{
    const u16 a0 = is_bit_set(col, 0) << 0;
    const u16 a1 = is_bit_set(col, 1) << 1;
    const u16 a2 = is_bit_set(col, 2) << 2;
    const u16 a3 = is_bit_set(col, 3) << 3;
    const u16 a4 = is_bit_set(col, 4) << 4;
    const u16 a5 = is_bit_set(pixel_line, 0) << 8;
    const u16 a6 = is_bit_set(pixel_line, 1) << 9;
    const u16 a7 = is_bit_set(pixel_line, 2) << 10;
    const u16 a8 = is_bit_set(row, 0) << 5;
    const u16 a9 = is_bit_set(row, 1) << 6;
    const u16 a10 = is_bit_set(row, 2) << 7;
    const u16 a11 = is_bit_set(row, 3) << 11;
    const u16 a12 = is_bit_set(row, 4) << 12;

    const u16 address = (0 << 15) | (1 << 14) | (0 << 13) | a12 | a11 | a7 | a6 | a5 | a10 | a9 | a8 | a4 | a3 | a2 | a1 | a0;

    return address - s_vram_offset;
}

u16 Gui::attribute_address_from_xy(u8 row, u8 col) {
    const u16 a0 = is_bit_set(col, 0) << 0;
    const u16 a1 = is_bit_set(col, 1) << 1;
    const u16 a2 = is_bit_set(col, 2) << 2;
    const u16 a3 = is_bit_set(col, 3) << 3;
    const u16 a4 = is_bit_set(col, 4) << 4;
    const u16 a8 = is_bit_set(row, 0) << 5;
    const u16 a9 = is_bit_set(row, 1) << 6;
    const u16 a10 = is_bit_set(row, 2) << 7;
    const u16 a11 = is_bit_set(row, 3) << 8;
    const u16 a12 = is_bit_set(row, 4) << 9;

    const u16 address = (0 << 15) | (1 << 14) | (0 << 13) | (1 << 12) | (1 << 11) | (0 << 10) | a12 | a11 | a10 | a9 | a8 | a4 | a3 | a2 | a1 | a0;

    return address - s_color_ram_offset;
}

void Gui::draw_attribute_blocks(Framebuffer& framebuffer, std::vector<u8> const& vram, std::vector<u8> const& color_ram)
{
    for (u8 row = 0; row < s_height_in_attribute_blocks; ++row) {
        for (u8 col = 0; col < s_width_in_attribute_blocks; ++col) {
            for (u8 pixel_line = 0; pixel_line < 8; ++pixel_line) {
                for (u8 bit_no = 0; bit_no < 8; ++bit_no) {
                    const u16 display_address = display_address_from_xy(row, col, pixel_line);
                    const u8 display_value = vram[display_address];
                    const unsigned int y = row * s_attribute_blocks_pixel_width + pixel_line + s_height_top_border;
                    const unsigned int x = col * s_attribute_blocks_pixel_width + (7 - bit_no) + s_width_left_border;

                    const u16 attribute_address = attribute_address_from_xy(row, col);
                    const u8 attribute_value = color_ram[attribute_address];
                    const bool is_bright = find_bright_mode(attribute_value);
                    const Color ink = find_ink(attribute_value, is_bright);
                    const Color paper = find_paper(attribute_value, is_bright);

                    framebuffer.set(y, x, is_bit_set(display_value, bit_no) ? ink : paper);
                }
            }
        }
    }
}

std::vector<u32> Gui::create_framebuffer(std::vector<u8> const& vram, std::vector<u8> const& color_ram, u8 border_color)
{
    if (!m_has_created_table) {
        throw std::runtime_error("Programming error: The lookup tables have to be made first. Run create_table() first.");
    }

    draw_borders(m_framebuffer, border_color);
    draw_attribute_blocks(m_framebuffer, vram, color_ram);

    return m_framebuffer.to_output_vector();
}
}
