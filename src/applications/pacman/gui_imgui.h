#pragma once

#include "crosscutting/gui/debugging_panes/cpu_info_pane.h"
#include "crosscutting/gui/debugging_panes/debug_log_pane.h"
#include "crosscutting/gui/debugging_panes/disassembly_pane.h"
#include "crosscutting/gui/debugging_panes/io_info_pane.h"
#include "crosscutting/gui/debugging_panes/memory_editor_pane.h"
#include "crosscutting/gui/debugging_panes/spritemap_pane.h"
#include "crosscutting/gui/debugging_panes/tilemap_pane.h"
#include "crosscutting/gui/debugging_panes/waveform_pane.h"
#include "crosscutting/misc/run_status.h"
#include "crosscutting/typedefs.h"
#include "gui.h"
#include <SDL_video.h>
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace emu::debugger {
template<class A, class D, std::size_t B>
class DebugContainer;
}
namespace emu::debugger {
template<class A, std::size_t B>
class Debugger;
}
namespace emu::logging {
class Logger;
}
namespace emu::z80 {
class GuiObserver;
}

namespace emu::applications::pacman {

using emu::gui::CpuInfoPane;
using emu::gui::DebugLogPane;
using emu::gui::DisassemblyPane;
using emu::gui::IoInfoPane;
using emu::gui::MemoryEditorPane;
using emu::gui::SpritemapPane;
using emu::gui::TilemapPane;
using emu::gui::WaveformPane;
using emu::misc::RunStatus;

class GuiImgui : public Gui {

public:
    GuiImgui();

    ~GuiImgui() override;

    void add_gui_observer(GuiObserver& observer) override;

    void remove_gui_observer(GuiObserver* observer) override;

    void update_screen(
        std::vector<u8> const& tile_ram,
        std::vector<u8> const& sprite_ram,
        std::vector<u8> const& palette_ram,
        bool is_screen_flipped,
        std::string const& game_window_subtitle) override;

    void update_debug_only() override;

    void attach_debugger(std::shared_ptr<Debugger<u16, 16>> debugger) override;

    void attach_debug_container(std::shared_ptr<DebugContainer<u16, u8, 16>> debug_container) override;

    void attach_logger(std::shared_ptr<Logger> logger) override;

    void toggle_tile_debug() override;

    void toggle_sprite_debug() override;

private:
    SDL_Window* m_win;
    SDL_GLContext m_gl_context;
    u32 m_screen_texture;
    u32 m_tile_texture;
    u32 m_sprite_texture;

    bool m_show_game;
    bool m_show_game_info;
    bool m_show_cpu_info;
    bool m_show_io_info;
    bool m_show_log;
    bool m_show_disassembly;
    bool m_show_memory_editor;
    bool m_show_tilemap;
    bool m_show_spritemap;
    bool m_show_waveforms;
    bool m_show_demo;

    bool m_is_in_debug_mode;

    std::vector<GuiObserver*> m_gui_observers;
    std::shared_ptr<Logger> m_logger;

    DebugLogPane m_log;
    DisassemblyPane<u16, u8, 16> m_disassembly;
    CpuInfoPane<u16, u8, 16> m_cpu_info;
    IoInfoPane<u16, u8, 16> m_io_info;
    MemoryEditorPane<u16, u8, 16> m_memory_editor;
    TilemapPane m_tilemap;
    SpritemapPane m_spritemap;
    WaveformPane m_waveforms;

    void notify_gui_observers_about_run_status(RunStatus new_status);

    void notify_gui_observers_about_debug_mode();

    void init();

    void render(std::string const& game_window_subtitle);

    void render_game_pane(std::string const& game_window_subtitle);

    void render_game_info_pane();

    void render_cpu_info_pane();

    void render_io_info_pane();

    void render_log_pane();

    void render_disassembly_pane();

    void render_memory_editor_pane();

    void render_tilemap_pane();

    void render_spritemap_pane();

    void render_waveform_pane();
};
}
