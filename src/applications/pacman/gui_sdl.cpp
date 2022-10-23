#include <iostream>
#include <SDL.h>
#include "gui_sdl.h"
#include "crosscutting/util/byte_util.h"

namespace emu::applications::pacman {

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

    void GuiSdl::attach_debugger([[maybe_unused]] std::shared_ptr<Debugger> debugger) {
    }

    void GuiSdl::attach_debug_container([[maybe_unused]] DebugContainer &debug_container) {
    }

    void GuiSdl::attach_logger([[maybe_unused]] std::shared_ptr<Logger> logger) {
    }

    void GuiSdl::toggle_tile_debug() {
    }

    void GuiSdl::toggle_sprite_debug() {
    }

    void GuiSdl::init() {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
            std::cerr << "error initializing SDL: " << SDL_GetError() << "\n";
            exit(1);
        }

        m_win = SDL_CreateWindow(
                "Pacman",
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

    void GuiSdl::update_screen(
            const std::vector<u8> &tile_ram,
            const std::vector<u8> &sprite_ram,
            const std::vector<u8> &palette_ram,
            RunStatus run_status,
            bool is_screen_flipped
    ) {
        std::vector<u32> framebuffer = create_framebuffer(tile_ram, sprite_ram, palette_ram, is_screen_flipped);

        void *pixels = nullptr;
        int pitch = 0;

        if (SDL_LockTexture(m_texture, nullptr, &pixels, &pitch) != 0) {
            std::cerr << "error while unlocking SDL texture: " << SDL_GetError() << "\n";
        } else {
            memcpy(pixels, framebuffer.data(), pitch * height);
        }

        std::string title;
        if (run_status == RunStatus::RUNNING) {
            title = "Pacman";
        } else if (run_status == RunStatus::PAUSED) {
            title = "Pacman - Paused";
        } else if (run_status == RunStatus::NOT_RUNNING) {
            title = "Pacman - Stopped";
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
