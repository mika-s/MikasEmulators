#include <iostream>
#include <SDL.h>
#include "gui_sdl.h"
#include "crosscutting/byte_util.h"

namespace emu::cpu8080::applications::space_invaders {

    using emu::util::byte::is_bit_set;

    GuiSdl::GuiSdl()
            : m_win(nullptr),
              m_rend(nullptr),
              m_texture(nullptr) {
        init();
    }

    GuiSdl::~GuiSdl() {
        SDL_DestroyTexture(m_texture);
        SDL_DestroyRenderer(m_rend);
        SDL_DestroyWindow(m_win);
        SDL_Quit();

        m_texture = nullptr;
        m_rend = nullptr;
        m_win = nullptr;
    }

    void GuiSdl::add_gui_observer(GuiObserver &observer) {
        m_gui_observers.push_back(&observer);
    }

    void GuiSdl::remove_gui_observer(GuiObserver *observer) {
        m_gui_observers.erase(
                std::remove(m_gui_observers.begin(), m_gui_observers.end(), observer),
                m_gui_observers.end()
        );
    }

    void GuiSdl::notify_gui_observers(RunStatus new_status) {
        for (GuiObserver *observer: m_gui_observers) {
            observer->run_status_changed(new_status);
        }
    }

    void GuiSdl::attach_debug_container([[maybe_unused]] DebugContainer &debug_container) {
    }

    void GuiSdl::init() {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
            std::cerr << "error initializing SDL: " << SDL_GetError() << "\n";
            exit(1);
        }

        m_win = SDL_CreateWindow(
                "Space Invaders",
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                scaled_width,
                scaled_height,
                SDL_WINDOW_RESIZABLE
        );
        if (!m_win) {
            std::cerr << "error creating SDL window: " << SDL_GetError() << "\n";
            exit(1);
        }

        m_rend = SDL_CreateRenderer(m_win, -1, SDL_RENDERER_ACCELERATED);

        if (!m_rend) {
            std::cerr << "error creating SDL renderer: " << SDL_GetError() << "\n";
            exit(1);
        }

        if (SDL_RenderSetScale(m_rend, scale, scale) != 0) {
            std::cerr << "error setting renderer scale in SDL: " << SDL_GetError() << "\n";
            exit(1);
        }

        m_texture = SDL_CreateTexture(m_rend, SDL_PIXELFORMAT_RGBA32,
                                      SDL_TEXTUREACCESS_STREAMING, width, height);

        if (!m_texture) {
            std::cerr << "error creating SDL texture: " << SDL_GetError() << "\n";
            exit(1);
        }
    }

    void GuiSdl::update_screen(const std::vector<std::uint8_t> &vram, [[maybe_unused]] RunStatus run_status) {
        std::uint8_t screen[height][width][colors + 1];

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

        void *pixels = nullptr;
        int pitch = 0;

        if (SDL_LockTexture(m_texture, nullptr, &pixels, &pitch) != 0) {
            std::cerr << "error while unlocking SDL texture: " << SDL_GetError() << "\n";
        } else {
            memcpy(pixels, screen, pitch * height);
        }

        std::string title;
        if (run_status == RunStatus::RUNNING) {
            title = "Space Invaders";
        } else if (run_status == RunStatus::PAUSED) {
            title = "Space Invaders - Paused";
        } else if (run_status == RunStatus::NOT_RUNNING) {
            title = "Space Invaders - Stopped";
        }

        SDL_SetWindowTitle(m_win, title.c_str());
        SDL_UnlockTexture(m_texture);
        SDL_RenderClear(m_rend);
        SDL_RenderCopy(m_rend, m_texture, nullptr, nullptr);
        SDL_RenderPresent(m_rend);
    }

    void GuiSdl::update_debug_only() {
    }
}
