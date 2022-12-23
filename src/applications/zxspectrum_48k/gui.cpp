#include "gui.h"
#include "crosscutting/util/byte_util.h"
#include <stdexcept>

namespace emu::applications::zxspectrum_48k {

using emu::util::byte::is_bit_set;

void Gui::create_table()
{
    m_lookup_table.reserve(256);

    // TODO

    m_has_created_table = true;
}

void Gui::draw_edges(Framebuffer& framebuffer)
{
    for (int row = 0; row < s_height; row++) {
        for (int col = 0; col < s_width_both_borders / 2; col++) {
            framebuffer.set(row, col, Color::black());
        }
    }

    for (int row = 0; row < s_height; row++) {
        for (int col = s_width - s_width_both_borders / 2; col < s_width; col++) {
            framebuffer.set(row, col, Color::black());
        }
    }

    for (int row = 0; row < s_height_both_borders / 2; row++) {
        for (int col = 0; col < s_width; col++) {
            framebuffer.set(row, col, Color::black());
        }
    }

    for (int row = s_height - s_height_both_borders / 2; row < s_height; row++) {
        for (int col = 0; col < s_width; col++) {
            framebuffer.set(row, col, Color::black());
        }
    }
}

void Gui::draw_attribute_blocks([[maybe_unused]] Framebuffer& framebuffer, [[maybe_unused]] std::vector<u8> const& vram, [[maybe_unused]] std::vector<u8> const& color_ram)
{
//    for (int y = 0; y < s_height; ++y) {
//        for (int block_idx = 0; block_idx < s_height_in_attribute_blocks; ++block_idx) {
//            u8 const pix = vram[m_lookup_table[y] + block_idx];
//            u8 const paper = color_ram[block_idx] & s_paper_mask;
//            u8 const ink = color_ram[block_idx] & s_ink_mask >> s_ink_shift;
//            bool const is_flash_mode = is_bit_set(color_ram[block_idx], s_flash_bit);
//            bool const is_bright_mode = is_bit_set(color_ram[block_idx], s_bright_bit);
//
//            for(int b = 0; b < 8; ++b) {
//                framebuffer.set(1, 1, ((pix << b) & 0x80) ? ink : paper);
//            }
//        }
//    }
}

std::vector<u32> Gui::create_framebuffer(std::vector<u8> const& vram, std::vector<u8> const& color_ram)
{
    if (!m_has_created_table) {
        throw std::runtime_error("Programming error: The lookup tables have to be made first. Run create_table() first.");
    }

    draw_edges(m_framebuffer);
    draw_attribute_blocks(m_framebuffer, vram, color_ram);

    return m_framebuffer.to_output_vector();
}
}
