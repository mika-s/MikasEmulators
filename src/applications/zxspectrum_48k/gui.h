#pragma once

#include "crosscutting/gui/graphics/color.h"
#include "crosscutting/gui/graphics/framebuffer.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace emu::applications::zxspectrum_48k {
class CpuIo;
class GuiObserver;
}
namespace emu::debugger {
template<class A, class D, std::size_t B>
class DebugContainer;
template<class A, std::size_t B>
class Debugger;
}
namespace emu::debugger {

}
namespace emu::logging {
class Logger;
}

namespace emu::applications::zxspectrum_48k {

using emu::debugger::DebugContainer;
using emu::debugger::Debugger;
using emu::gui::Color;
using emu::gui::Framebuffer;
using emu::logging::Logger;
using emu::util::byte::is_bit_set;
using emu::util::byte::to_u32;

class Gui {
public:
    virtual ~Gui() = default;

    void create_table();

    virtual void add_gui_observer(GuiObserver& observer) = 0;

    virtual void remove_gui_observer(GuiObserver* observer) = 0;

    virtual void update_screen(
        std::vector<u8> const& vram,
        std::vector<u8> const& color_ram,
        u8 border_color,
        std::string const& game_window_subtitle)
        = 0;

    virtual void update_debug_only() = 0;

    virtual void attach_debugger(std::shared_ptr<Debugger<u16, 16>> debugger) = 0;

    virtual void attach_debug_container(std::shared_ptr<DebugContainer<u16, u8, 16>> debug_container) = 0;

    virtual void attach_cpu_io(CpuIo const* cpu_io) = 0;

    virtual void attach_logger(std::shared_ptr<Logger> logger) = 0;

protected:
    static constexpr int s_flash_bit = 7;
    static constexpr int s_bright_bit = 6;
    static constexpr unsigned int s_ink_mask = 0b00000111;
    static constexpr unsigned int s_paper_mask = 0b00111000;
    static constexpr unsigned int s_paper_shift = 3;
    static constexpr u8 s_or = 0xd0;
    static constexpr u8 s_br = 0xff;

    static const inline std::vector<Color> s_ordinary_colors = {
        Color(0xff, 0x00, 0x00, 0x00), // black
        Color(0xff, 0x00, 0x00, s_or), // blue
        Color(0xff, s_or, 0x00, 0x00), // red
        Color(0xff, s_or, 0x00, s_or), // magenta
        Color(0xff, 0x00, s_or, 0x00), // green
        Color(0xff, 0x00, s_or, s_or), // cyan
        Color(0xff, s_or, s_or, 0x00), // yellow
        Color(0xff, s_or, s_or, s_or)  // white
    };

    static const inline std::vector<Color> s_bright_colors = {
        Color(0xff, 0x00, 0x00, 0x00), // black
        Color(0xff, 0x00, 0x00, s_br), // blue
        Color(0xff, s_br, 0x00, 0x00), // red
        Color(0xff, s_br, 0x00, s_br), // magenta
        Color(0xff, 0x00, s_br, 0x00), // green
        Color(0xff, 0x00, s_br, s_br), // cyan
        Color(0xff, s_br, s_br, 0x00), // yellow
        Color(0xff, s_br, s_br, s_br)  // white
    };

    static constexpr int s_width_in_attribute_blocks = 32;
    static constexpr int s_height_in_attribute_blocks = 24;
    static constexpr int s_attribute_blocks_pixel_width = 8;
    static constexpr int s_width_left_border = 32;
    static constexpr int s_width_right_border = 32;
    static constexpr int s_width_both_borders = s_width_left_border + s_width_right_border;
    static constexpr int s_width_visible_area = 256; // 32 * 8
    static constexpr int s_height_top_border = 24;
    static constexpr int s_height_bottom_border = 24;
    static constexpr int s_height_both_borders = s_height_top_border + s_height_bottom_border;
    static constexpr int s_height_visible_area = 192; // 24 * 8

    // Final width and height
    static constexpr int s_width = s_width_visible_area + s_width_both_borders;
    static constexpr int s_height = s_height_visible_area + s_height_both_borders;

    static constexpr float s_scale = 4.0;
    static constexpr int s_scaled_width = static_cast<int>(s_scale * static_cast<float>(s_width));
    static constexpr int s_scaled_height = static_cast<int>(s_scale * static_cast<float>(s_height));

    static constexpr u16 s_vram_offset = 0x4000;
    static constexpr u16 s_color_ram_offset = 0x5800;

    bool m_has_created_table { false };

    Framebuffer m_framebuffer { Framebuffer(s_height, s_width, Color(0xff, 0, 128, 255)) };

    std::vector<int> m_lookup_table;

    static void draw_borders(Framebuffer& framebuffer, u8 border_color);

    Color find_ink(u8 value, bool is_bright);

    Color find_paper(u8 value, bool is_bright);

    bool find_flash_mode(u8 value);

    bool find_bright_mode(u8 value);

    u16 display_address_from_xy(u8 row, u8 col, u8 pixel_line);

    u16 attribute_address_from_xy(u8 row, u8 col);

    void draw_attribute_blocks(Framebuffer& framebuffer, std::vector<u8> const& vram, std::vector<u8> const& color_ram);

    std::vector<u32> create_framebuffer(std::vector<u8> const& vram, std::vector<u8> const& color_ram, u8 border_color);
};
}
