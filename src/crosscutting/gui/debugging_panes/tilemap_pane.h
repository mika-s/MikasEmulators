#ifndef MIKA_EMULATORS_CROSSCUTTING_GUI_DEBUGGING_PANES_TILEMAP_PANE_H
#define MIKA_EMULATORS_CROSSCUTTING_GUI_DEBUGGING_PANES_TILEMAP_PANE_H

#include <string>
#include <vector>
#include "imgui.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/debugging/debug_container.h"
#include "crosscutting/gui/graphics/framebuffer.h"

namespace emu::gui {

    using emu::debugger::DebugContainer;

    class TilemapPane {
    public:
        explicit TilemapPane(int default_palette_idx = 0);

        void attach_debug_container(DebugContainer &debug_container);

        void draw(const char *title, u32 tile_texture, bool *p_open = nullptr);

    private:
        static constexpr int width = 128;
        static constexpr int height = 128;
        static constexpr unsigned int tiles_per_row = 16;

        static constexpr float scale = 4.0;
        static constexpr int scaled_width = static_cast<int>(scale * static_cast<float>(width));
        static constexpr int scaled_height = static_cast<int>(scale * static_cast<float>(height));

        DebugContainer m_debug_container;
        bool m_is_debug_container_set;
        std::vector<Framebuffer> m_framebuffers;
        int m_chosen_palette_idx;
        int m_number_of_palettes;
        ImGuiSliderFlags m_slider_flags;
        bool m_are_all_tiles_rendered;

        void prepare_framebuffers();

        bool prepare_framebuffer(unsigned int palette_idx);

        void render_image(u32 tile_texture);
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_GUI_DEBUGGING_PANES_TILEMAP_PANE_H
