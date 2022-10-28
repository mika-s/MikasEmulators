#include <iostream>
#include <SDL.h>
#include "gui_sdl.h"
#include "crosscutting/util/byte_util.h"

namespace emu::applications::space_invaders {

    using emu::i8080::RunStatus::NOT_RUNNING;
    using emu::i8080::RunStatus::RUNNING;
    using emu::i8080::RunStatus::PAUSED;
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

    void GuiSdl::init() {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "error initializing SDL: %s", SDL_GetError());
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
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "error creating SDL window: %s", SDL_GetError());
            exit(1);
        }

        m_rend = SDL_CreateRenderer(m_win, -1, SDL_RENDERER_ACCELERATED);

        if (!m_rend) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "error creating SDL renderer: %s", SDL_GetError());
            exit(1);
        }

        if (SDL_RenderSetScale(m_rend, scale, scale) != 0) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "error setting renderer scale in SDL: %s", SDL_GetError());
            exit(1);
        }

        m_texture = SDL_CreateTexture(m_rend, SDL_PIXELFORMAT_RGBA32,
                                      SDL_TEXTUREACCESS_STREAMING, width, height);

        if (!m_texture) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "error creating SDL texture: %s", SDL_GetError());
            exit(1);
        }
    }

    void GuiSdl::update_screen(const std::vector<u8> &vram, RunStatus run_status) {
        std::vector<u32> framebuffer = create_framebuffer(vram);

        void *pixels = nullptr;
        int pitch = 0;

        if (SDL_LockTexture(m_texture, nullptr, &pixels, &pitch) != 0) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "error while unlocking SDL texture: %s", SDL_GetError());
        } else {
            memcpy(pixels, framebuffer.data(), static_cast<std::size_t>(pitch * height));
        }

        std::string title;
        if (run_status == RUNNING) {
            title = "Space Invaders";
        } else if (run_status == PAUSED) {
            title = "Space Invaders - Paused";
        } else if (run_status == NOT_RUNNING) {
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
