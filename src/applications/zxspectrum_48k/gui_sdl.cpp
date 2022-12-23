#include "gui_sdl.h"
#include "gui.h"
#include <SDL.h>
#include <SDL_error.h>
#include <SDL_log.h>
#include <SDL_pixels.h>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <string>

namespace emu::applications::zxspectrum_48k {
class GuiObserver;
}
namespace emu::debugger {
template<class A, class D, std::size_t B>
class DebugContainer;
template<class A, std::size_t B>
class Debugger;
}
namespace emu::logging {
class Logger;
}

namespace emu::applications::zxspectrum_48k {

GuiSdl::GuiSdl()
    : m_win(nullptr)
    , m_rend(nullptr)
    , m_texture(nullptr)
{
    init();
}

GuiSdl::~GuiSdl()
{
    SDL_DestroyTexture(m_texture);
    SDL_DestroyRenderer(m_rend);
    SDL_DestroyWindow(m_win);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);

    m_texture = nullptr;
    m_rend = nullptr;
    m_win = nullptr;
}

void GuiSdl::add_gui_observer(GuiObserver& observer)
{
    m_gui_observers.push_back(&observer);
}

void GuiSdl::remove_gui_observer(GuiObserver* observer)
{
    m_gui_observers.erase(
        std::remove(m_gui_observers.begin(), m_gui_observers.end(), observer),
        m_gui_observers.end());
}

void GuiSdl::attach_debugger([[maybe_unused]] std::shared_ptr<Debugger<u16, 16>> debugger)
{
}

void GuiSdl::attach_debug_container([[maybe_unused]] std::shared_ptr<DebugContainer<u16, u8, 16>> debug_container)
{
}

void GuiSdl::attach_logger([[maybe_unused]] std::shared_ptr<Logger> logger)
{
}

void GuiSdl::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "error initializing SDL: %s", SDL_GetError());
        exit(1);
    }

    m_win = SDL_CreateWindow(
        "ZX Spectrum 48k",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        s_scaled_width,
        s_scaled_height,
        SDL_WINDOW_RESIZABLE);
    if (!m_win) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "error creating SDL window: %s", SDL_GetError());
        exit(1);
    }

    m_rend = SDL_CreateRenderer(m_win, -1, SDL_RENDERER_ACCELERATED);

    if (!m_rend) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "error creating SDL renderer: %s", SDL_GetError());
        exit(1);
    }

    if (SDL_RenderSetScale(m_rend, s_scale, s_scale) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "error setting renderer s_scale in SDL: %s", SDL_GetError());
        exit(1);
    }

    m_texture = SDL_CreateTexture(m_rend, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, s_width, s_height);

    if (!m_texture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "error creating SDL texture: %s", SDL_GetError());
        exit(1);
    }
}

void GuiSdl::update_screen(std::vector<u8> const& vram, std::vector<u8> const& color_ram, std::string const& game_window_subtitle)
{
    std::vector<u32> framebuffer = create_framebuffer(vram, color_ram);

    void* pixels = nullptr;
    int pitch = 0;

    if (SDL_LockTexture(m_texture, nullptr, &pixels, &pitch) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "error while unlocking SDL texture: %s", SDL_GetError());
    } else {
        memcpy(pixels, framebuffer.data(), static_cast<std::size_t>(pitch * s_height));
    }

    const std::string title = game_window_subtitle.empty() ? "ZX Spectrum 48k" : "ZX Spectrum 48k - " + game_window_subtitle;

    SDL_SetWindowTitle(m_win, title.c_str());
    SDL_UnlockTexture(m_texture);
    SDL_RenderClear(m_rend);
    SDL_RenderCopy(m_rend, m_texture, nullptr, nullptr);
    SDL_RenderPresent(m_rend);
}

void GuiSdl::update_debug_only()
{
}
}
