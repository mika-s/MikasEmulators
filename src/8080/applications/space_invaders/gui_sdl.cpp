#include <iostream>
#include <SDL.h>
#include "gui_sdl.h"
#include "crosscutting/byte_util.h"

namespace emu::cpu8080::applications::space_invaders {

    using emu::util::byte::is_bit_set;

    GuiSdl::GuiSdl()
            : win(nullptr),
              rend(nullptr),
              texture(nullptr) {
        init();
    }

    GuiSdl::~GuiSdl() {
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();

        texture = nullptr;
        rend = nullptr;
        win = nullptr;
    }

    void GuiSdl::init() {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
            std::cerr << "error initializing SDL: " << SDL_GetError() << "\n";
            exit(1);
        }

        win = SDL_CreateWindow(
                "Space Invaders",
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                scaled_width,
                scaled_height,
                SDL_WINDOW_RESIZABLE
        );
        if (!win) {
            std::cerr << "error creating SDL window: " << SDL_GetError() << "\n";
            exit(1);
        }

        rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

        if (!rend) {
            std::cerr << "error creating SDL renderer: " << SDL_GetError() << "\n";
            exit(1);
        }

        if (SDL_RenderSetScale(rend, scale, scale) != 0) {
            std::cerr << "error setting renderer scale in SDL: " << SDL_GetError() << "\n";
            exit(1);
        }

        texture = SDL_CreateTexture(rend, SDL_PIXELFORMAT_RGBA32,
                                    SDL_TEXTUREACCESS_STREAMING, width, height);

        if (!texture) {
            std::cerr << "error creating SDL texture: " << SDL_GetError() << "\n";
            exit(1);
        }
    }

    void GuiSdl::update_screen(const std::vector <std::uint8_t> &vram) {
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

        if (SDL_LockTexture(texture, nullptr, &pixels, &pitch) != 0) {
            std::cerr << "error while unlocking SDL texture: " << SDL_GetError() << "\n";
        } else {
            memcpy(pixels, screen, pitch * height);
        }

        SDL_UnlockTexture(texture);

        SDL_RenderClear(rend);
        SDL_RenderCopy(rend, texture, nullptr, nullptr);
        SDL_RenderPresent(rend);
    }
}
