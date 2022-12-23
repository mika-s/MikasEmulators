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

    virtual void update_screen(std::vector<u8> const& vram, std::vector<u8> const& color_ram, std::string const& game_window_subtitle) = 0;

    virtual void update_debug_only() = 0;

    virtual void attach_debugger(std::shared_ptr<Debugger<u16, 16>> debugger) = 0;

    virtual void attach_debug_container(std::shared_ptr<DebugContainer<u16, u8, 16>> debug_container) = 0;

    virtual void attach_logger(std::shared_ptr<Logger> logger) = 0;

protected:
    static constexpr int s_flash_bit = 7;
    static constexpr int s_bright_bit = 6;
    static constexpr unsigned int s_paper_mask = 0b00000111;
    static constexpr unsigned int s_ink_mask = 0b00111000;
    static constexpr unsigned int s_ink_shift = 3;

    static const inline std::vector<Color> s_colors = {
        Color(0xff, 0, 0, 0),
        Color(0xff, 0, 0, 0xff),
        Color(0xff, 0xff, 0, 0),
        Color(0xff, 0xff, 0, 0xff),
        Color(0xff, 0, 0xff, 0),
        Color(0xff, 0, 0xff, 0xff),
        Color(0xff, 0xff, 0xff, 0),
        Color(0xff, 0xff, 0xff, 0xff)
    };

    static constexpr int s_bits_in_byte = 8;
    static constexpr int s_width_in_attribute_blocks = 32;
    static constexpr int s_height_in_attribute_blocks = 24;
    static constexpr int s_attribute_blocks_pixel_width = 8;
    static constexpr int s_width_both_borders = 64;
    static constexpr int s_width_visible_area = 256; // 32 * 8
    static constexpr int s_height_both_borders = 48;
    static constexpr int s_height_visible_area = 192; // 24 * 8

    // Final width and height
    static constexpr int s_width = s_width_visible_area + s_width_both_borders;
    static constexpr int s_height = s_height_visible_area + s_height_both_borders;

    static constexpr float s_scale = 4.0;
    static constexpr int s_scaled_width = static_cast<int>(s_scale * static_cast<float>(s_width));
    static constexpr int s_scaled_height = static_cast<int>(s_scale * static_cast<float>(s_height));

    bool m_has_created_table { false };

    Framebuffer m_framebuffer { Framebuffer(s_height, s_width, Color(0xff, 0, 128, 255)) };

    std::vector<int> m_lookup_table;

    static void draw_edges(Framebuffer& framebuffer);

    void draw_attribute_blocks(Framebuffer& framebuffer, std::vector<u8> const& vram, std::vector<u8> const& color_ram);

    std::vector<u32> create_framebuffer(std::vector<u8> const& vram, std::vector<u8> const& color_ram);
};
}
