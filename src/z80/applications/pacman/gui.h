#ifndef MIKA_EMULATORS_Z80_APPLICATIONS_PACMAN_GUI_H
#define MIKA_EMULATORS_Z80_APPLICATIONS_PACMAN_GUI_H

#include <memory>
#include "z80/debug_container.h"
#include "z80/interfaces/gui_observer.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/debugging/debugger.h"
#include "crosscutting/logging/logger.h"
#include "crosscutting/util/byte_util.h"

namespace emu::z80::applications::pacman {

    using emu::util::byte::is_bit_set;
    using emu::debugger::Debugger;
    using emu::logging::Logger;

    class Gui {
    public:
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

        static std::vector<u32> create_framebuffer(const std::vector<u8> &vram) {
            u8 screen[height][width][colors];

            for (int i = 0; i < height * width / bits_in_byte; i++) {
                const int y = i * bits_in_byte / height;
                const int base_x = (i * bits_in_byte) % height;
                const u8 current_byte = vram[i];

                for (u8 bit = 0; bit < bits_in_byte; bit++) {
                    int px = base_x + bit;
                    int py = y;
                    const bool is_pixel_lit = is_bit_set(current_byte, bit);
                    u8 r = 0;
                    u8 g = 0;
                    u8 b = 0;

                    if (is_pixel_lit) {
                        if (px < 16) {
                            if (py < 16 || 134 < py) {
                                r = 255;
                                g = 255;
                                b = 255;
                            } else {
                                g = 255;
                            }
                        } else if (16 <= px && px <= 72) {
                            g = 255;
                        } else if (192 <= px && px < 224) {
                            r = 255;
                        } else {
                            r = 255;
                            g = 255;
                            b = 255;
                        }
                    }

                    const int temp_x = px;
                    px = py;
                    py = -temp_x + height - 1;

                    screen[py][px][0] = r;
                    screen[py][px][1] = g;
                    screen[py][px][2] = b;
                }
            }

            std::vector<u32> output;

            for (auto &height_idx: screen) {
                for (auto &width_idx: height_idx) {
                    u8 r = width_idx[0];
                    u8 g = width_idx[1];
                    u8 b = width_idx[2];
                    output.push_back(0xff000000 | b << 16 | g << 8 | r);
                }
            }

            return output;
        }
    };
}

#endif //MIKA_EMULATORS_Z80_APPLICATIONS_PACMAN_GUI_H
