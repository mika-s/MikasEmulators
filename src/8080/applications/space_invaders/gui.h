#ifndef MIKA_EMULATORS_8080_APPLICATIONS_SPACE_INVADERS_GUI_H
#define MIKA_EMULATORS_8080_APPLICATIONS_SPACE_INVADERS_GUI_H

#include "8080/interfaces/gui_observer.h"
#include "crosscutting/byte_util.h"

namespace emu::cpu8080::applications::space_invaders {

    using emu::util::byte::is_bit_set;

    class Gui {
    public:
        virtual ~Gui() = default;

        virtual void add_gui_observer(GuiObserver &observer) = 0;

        virtual void remove_gui_observer(GuiObserver *observer) = 0;

        virtual void update_screen(const std::vector<std::uint8_t> &vram, RunStatus run_status) = 0;

        virtual void update_debug_only() = 0;

        virtual void attach_debug_container(DebugContainer &debug_container) = 0;

    protected:
        static constexpr int bits_in_byte = 8;
        static constexpr float scale = 4.0;
        static constexpr int width = 224;
        static constexpr int height = 256;
        static constexpr int colors = 3;
        static constexpr int scaled_width = static_cast<int>(scale * static_cast<float>(width));
        static constexpr int scaled_height = static_cast<int>(scale * static_cast<float>(height));

        static std::vector<std::uint32_t> create_framebuffer(const std::vector<std::uint8_t> &vram) {
            std::uint8_t screen[height][width][colors];

            for (int i = 0; i < height * width / bits_in_byte; i++) {
                const int y = i * bits_in_byte / height;
                const int base_x = (i * bits_in_byte) % height;
                const std::uint8_t current_byte = vram[i];

                for (std::uint8_t bit = 0; bit < bits_in_byte; bit++) {
                    int px = base_x + bit;
                    int py = y;
                    const bool is_pixel_lit = is_bit_set(current_byte, bit);
                    std::uint8_t r = 0;
                    std::uint8_t g = 0;
                    std::uint8_t b = 0;

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

            std::vector<std::uint32_t> output;

            for (auto &height_idx: screen) {
                for (auto &width_idx: height_idx) {
                    std::uint8_t r = width_idx[0];
                    std::uint8_t g = width_idx[1];
                    std::uint8_t b = width_idx[2];
                    output.push_back(0xFF000000 | b << 16 | g << 8 | r);
                }
            }

            return output;
        }
    };
}

#endif //MIKA_EMULATORS_8080_APPLICATIONS_SPACE_INVADERS_GUI_H
