#pragma once

#include "chips/8080/interfaces/gui_observer.h"
#include "crosscutting/debugging/debug_container.h"
#include "crosscutting/debugging/debugger.h"
#include "crosscutting/gui/graphics/color.h"
#include "crosscutting/gui/graphics/framebuffer.h"
#include "crosscutting/logging/logger.h"
#include "crosscutting/misc/run_status.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"

namespace emu::applications::space_invaders {

using emu::debugger::DebugContainer;
using emu::debugger::Debugger;
using emu::gui::Color;
using emu::gui::Framebuffer;
using emu::i8080::GuiObserver;
using emu::logging::Logger;
using emu::misc::RunStatus;
using emu::util::byte::is_bit_set;
using emu::util::byte::to_u32;

class Gui {
public:
    Gui()
        : m_framebuffer(Framebuffer(s_height, s_width, Color(0xff, 0, 128, 255)))
    {
    }

    virtual ~Gui() = default;

    virtual void add_gui_observer(GuiObserver& observer) = 0;

    virtual void remove_gui_observer(GuiObserver* observer) = 0;

    virtual void update_screen(std::vector<u8> const& vram, std::string const& game_window_subtitle) = 0;

    virtual void update_debug_only() = 0;

    virtual void attach_debugger(std::shared_ptr<Debugger<u16, 16>> debugger) = 0;

    virtual void attach_debug_container(std::shared_ptr<DebugContainer<u16, u8, 16>> debug_container) = 0;

    virtual void attach_logger(std::shared_ptr<Logger> logger) = 0;

protected:
    static constexpr int s_bits_in_byte = 8;
    static constexpr float s_scale = 4.0;
    static constexpr int s_width = 224;
    static constexpr int s_height = 256;
    static constexpr int s_scaled_width = static_cast<int>(s_scale * static_cast<float>(s_width));
    static constexpr int s_scaled_height = static_cast<int>(s_scale * static_cast<float>(s_height));

    Framebuffer m_framebuffer;

    std::vector<u32> create_framebuffer(std::vector<u8> const& vram)
    {
        for (int i = 0; i < s_height * s_width / s_bits_in_byte; ++i) {
            int const y = i * s_bits_in_byte / s_height;
            int const base_x = (i * s_bits_in_byte) % s_height;
            const u8 current_byte = vram[static_cast<std::size_t>(i)];

            for (u8 bit = 0; bit < s_bits_in_byte; ++bit) {
                int px = base_x + bit;
                int py = y;
                bool const is_pixel_lit = is_bit_set(current_byte, bit);
                u8 r = 0, g = 0, b = 0;

                if (is_pixel_lit) {
                    if (px < 16) {
                        if (py < 16 || 134 < py) {
                            r = g = b = 255;
                        } else {
                            g = 255;
                        }
                    } else if (px <= 72) {
                        g = 255;
                    } else if (192 <= px && px < 224) {
                        r = 255;
                    } else {
                        r = g = b = 255;
                    }
                }

                int const temp_x = px;
                px = py;
                py = -temp_x + s_height - 1;

                m_framebuffer.set(
                    static_cast<unsigned int>(py),
                    static_cast<unsigned int>(px),
                    Color(0xff, r, g, b));
            }
        }

        return m_framebuffer.to_output_vector();
    }
};
}
