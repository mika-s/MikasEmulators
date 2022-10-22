#ifndef MIKA_EMULATORS_APPLICATIONS_SPACE_INVADERS_GUI_H
#define MIKA_EMULATORS_APPLICATIONS_SPACE_INVADERS_GUI_H

#include "chips/8080/interfaces/gui_observer.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/debugging/debugger.h"
#include "crosscutting/debugging/debug_container.h"
#include "crosscutting/gui/graphics/color.h"
#include "crosscutting/gui/graphics/framebuffer.h"
#include "crosscutting/logging/logger.h"
#include "crosscutting/misc/run_status.h"
#include "crosscutting/util/byte_util.h"

namespace emu::applications::space_invaders {

    using emu::util::byte::is_bit_set;
    using emu::util::byte::to_u32;
    using emu::debugger::DebugContainer;
    using emu::debugger::Debugger;
    using emu::gui::Color;
    using emu::gui::Framebuffer;
    using emu::logging::Logger;
    using emu::i8080::GuiObserver;
    using emu::i8080::RunStatus;

    class Gui {
    public:
        Gui()
                : m_framebuffer(Framebuffer(height, width, Color(0xff, 0, 128, 255))) {
        }

        virtual ~Gui() = default;

        virtual void add_gui_observer(GuiObserver &observer) = 0;

        virtual void remove_gui_observer(GuiObserver *observer) = 0;

        virtual void update_screen(const std::vector<u8> &vram, RunStatus run_status) = 0;

        virtual void update_debug_only() = 0;

        virtual void attach_debugger(std::shared_ptr<Debugger> debugger) = 0;

        virtual void attach_debug_container(DebugContainer &debug_container) = 0;

        virtual void attach_logger(std::shared_ptr<Logger> logger) = 0;

    protected:
        static constexpr int bits_in_byte = 8;
        static constexpr float scale = 4.0;
        static constexpr int width = 224;
        static constexpr int height = 256;
        static constexpr int colors = 3;
        static constexpr int scaled_width = static_cast<int>(scale * static_cast<float>(width));
        static constexpr int scaled_height = static_cast<int>(scale * static_cast<float>(height));

        Framebuffer m_framebuffer;

        std::vector<u32> create_framebuffer(const std::vector<u8> &vram) {
            for (int i = 0; i < height * width / bits_in_byte; ++i) {
                const int y = i * bits_in_byte / height;
                const int base_x = (i * bits_in_byte) % height;
                const u8 current_byte = vram[static_cast<std::size_t>(i)];

                for (u8 bit = 0; bit < bits_in_byte; ++bit) {
                    int px = base_x + bit;
                    int py = y;
                    const bool is_pixel_lit = is_bit_set(current_byte, bit);
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

                    const int temp_x = px;
                    px = py;
                    py = -temp_x + height - 1;

                    m_framebuffer.set(
                                static_cast<unsigned int>(py),
                                static_cast<unsigned int>(px),
                                Color(0xff, r, g, b)
                    );
                }
            }

            return m_framebuffer.to_output_vector();
        }
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_SPACE_INVADERS_GUI_H
