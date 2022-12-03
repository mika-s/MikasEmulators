#ifndef MIKA_EMULATORS_APPLICATIONS_PACMAN_GUI_IMGUI_H
#define MIKA_EMULATORS_APPLICATIONS_PACMAN_GUI_IMGUI_H

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
#include <memory>
#include <vector>

namespace emu::debugger {
    template<class A, class D>
    class DebugContainer;
}
namespace emu::debugger {
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

        void add_gui_observer(GuiObserver &observer) override;

        void remove_gui_observer(GuiObserver *observer) override;

        void update_screen(
                const std::vector<u8> &tile_ram,
                const std::vector<u8> &sprite_ram,
                const std::vector<u8> &palette_ram,
                RunStatus run_status,
                bool is_screen_flipped
        ) override;

        void update_debug_only() override;

        void attach_debugger(std::shared_ptr<Debugger> debugger) override;

        void attach_debug_container(DebugContainer<u16, u8> &debug_container) override;

        void attach_logger(std::shared_ptr<Logger> logger) override;

        void toggle_tile_debug() override;

        void toggle_sprite_debug() override;

    private:
        SDL_Window *m_win;
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

        std::vector<GuiObserver *> m_gui_observers;
        std::shared_ptr<Logger> m_logger;

        DebugLogPane m_log;
        DisassemblyPane m_disassembly;
        CpuInfoPane m_cpu_info;
        IoInfoPane m_io_info;
        MemoryEditorPane m_memory_editor;
        TilemapPane m_tilemap;
        SpritemapPane m_spritemap;
        WaveformPane m_waveforms;

        void notify_gui_observers_about_run_status(RunStatus new_status);

        void notify_gui_observers_about_debug_mode();

        void init();

        void render(RunStatus run_status);

        void render_game_pane(RunStatus run_status);

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

#endif //MIKA_EMULATORS_APPLICATIONS_PACMAN_GUI_IMGUI_H
