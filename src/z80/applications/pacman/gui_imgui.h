#ifndef MIKA_EMULATORS_Z80_APPLICATIONS_PACMAN_GUI_GUI_IMGUI_H
#define MIKA_EMULATORS_Z80_APPLICATIONS_PACMAN_GUI_GUI_IMGUI_H

#include <cstdint>
#include <memory>
#include <vector>
#include <SDL_video.h>
#include <SDL_render.h>
#include "gui.h"
#include "z80/run_status.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/debugging/debugger.h"
#include "crosscutting/debugging/debug_container.h"
#include "crosscutting/gui/cpu_info.h"
#include "crosscutting/gui/debug_log.h"
#include "crosscutting/gui/disassembly_window.h"
#include "crosscutting/gui/io_info.h"
#include "crosscutting/gui/memory_editor_window.h"
#include "crosscutting/gui/tilemap_window.h"
#include "crosscutting/gui/spritemap_window.h"
#include "crosscutting/logging/log_observer.h"
#include "crosscutting/logging/logger.h"

namespace emu::z80::applications::pacman {

    using emu::gui::DebugLog;
    using emu::gui::DisassemblyWindow;
    using emu::gui::CpuInfo;
    using emu::gui::IoInfo;
    using emu::gui::MemoryEditorWindow;
    using emu::gui::TilemapWindow;
    using emu::gui::SpritemapWindow;

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
                RunStatus run_status
        ) override;

        void update_debug_only() override;

        void attach_debugger(std::shared_ptr<Debugger> debugger) override;

        void attach_debug_container(DebugContainer &debug_container) override;

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
        bool m_show_demo;

        bool m_is_in_debug_mode;

        std::vector<GuiObserver *> m_gui_observers;
        std::shared_ptr<Logger> m_logger;

        DebugLog m_log;
        DisassemblyWindow m_disassembly;
        CpuInfo m_cpu_info;
        IoInfo m_io_info;
        MemoryEditorWindow m_memory_editor;
        TilemapWindow m_tilemap;
        SpritemapWindow m_spritemap;

        void notify_gui_observers_about_run_status(RunStatus new_status);

        void notify_gui_observers_about_debug_mode();

        void init();

        void render(RunStatus run_status);

        void render_game_window(RunStatus run_status);

        void render_game_info_window();

        void render_cpu_info_window();

        void render_io_info_window();

        void render_log_window();

        void render_disassembly_window();

        void render_memory_editor_window();

        void render_tilemap_window();

        void render_spritemap_window();
    };
}

#endif //MIKA_EMULATORS_Z80_APPLICATIONS_PACMAN_GUI_GUI_IMGUI_H
