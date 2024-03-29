#pragma once

#include "crosscutting/gui/graphics/framebuffer.h"
#include "crosscutting/typedefs.h"
#include "imgui.h"
#include <cstddef>
#include <memory>
#include <vector>

namespace emu::debugger {
template<class A, class D, std::size_t B>
class DebugContainer;
}

namespace emu::gui {

using emu::debugger::DebugContainer;

class TilemapPane {
public:
    explicit TilemapPane(int default_palette_idx = 0);

    void attach_debug_container(std::shared_ptr<DebugContainer<u16, u8, 16>> debug_container);

    void draw(char const* title, u32 tile_texture, bool* p_open = nullptr);

private:
    static constexpr int width = 128;
    static constexpr int height = 128;
    static constexpr unsigned int tiles_per_row = 16;

    static constexpr float scale = 4.0;
    static constexpr int scaled_width = static_cast<int>(scale * static_cast<float>(width));
    static constexpr int scaled_height = static_cast<int>(scale * static_cast<float>(height));

    std::shared_ptr<DebugContainer<u16, u8, 16>> m_debug_container;
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
