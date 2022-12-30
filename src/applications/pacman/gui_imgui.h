#pragma once

#include "crosscutting/gui/debugging_panes/cpu_info_pane.h"
#include "crosscutting/gui/debugging_panes/debug_log_pane.h"
#include "crosscutting/gui/debugging_panes/disassembly_pane.h"
#include "crosscutting/gui/debugging_panes/io_info_pane.h"
#include "crosscutting/gui/debugging_panes/memory_editor_pane.h"
#include "crosscutting/gui/debugging_panes/spritemap_pane.h"
#include "crosscutting/gui/debugging_panes/tilemap_pane.h"
#include "crosscutting/gui/debugging_panes/waveform_pane.h"
#include "crosscutting/typedefs.h"
#include "gui.h"
#include <SDL_video.h>
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace emu::applications::pacman {
class GuiObserver;
struct GuiRequest;
}
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

namespace emu::applications::pacman {

using emu::gui::CpuInfoPane;
using emu::gui::DebugLogPane;
using emu::gui::DisassemblyPane;
using emu::gui::IoInfoPane;
using emu::gui::MemoryEditorPane;
using emu::gui::SpritemapPane;
using emu::gui::TilemapPane;
using emu::gui::WaveformPane;

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
    SDL_Window* m_win { nullptr };
    SDL_GLContext m_gl_context { nullptr };
    u32 m_screen_texture { 0 };
    u32 m_tile_texture { 0 };
    u32 m_sprite_texture { 0 };

    bool m_show_game { true };
    bool m_show_game_info { true };
    bool m_show_cpu_info { true };
    bool m_show_io_info { true };
    bool m_show_log { true };
    bool m_show_disassembly { true };
    bool m_show_memory_editor { true };
    bool m_show_tilemap { true };
    bool m_show_spritemap { true };
    bool m_show_waveforms { true };
    bool m_show_demo { false };

    bool m_is_in_debug_mode { false };

    std::vector<GuiObserver*> m_gui_observers;
    std::shared_ptr<Logger> m_logger;

    DebugLogPane m_log;
    DisassemblyPane<u16, u8, 16> m_disassembly;
    CpuInfoPane<u16, u8, 16> m_cpu_info;
    IoInfoPane<u16, u8, 16> m_io_info;
    MemoryEditorPane<u16, u8, 16> m_memory_editor;
    TilemapPane m_tilemap { 1 };
    SpritemapPane m_spritemap { 1 };
    WaveformPane m_waveforms;

    void notify_gui_observers(GuiRequest request);

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
