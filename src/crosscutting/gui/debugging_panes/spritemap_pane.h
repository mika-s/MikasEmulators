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
class Sprite;
}

namespace emu::gui {

using emu::debugger::DebugContainer;

class SpritemapPane {
public:
    explicit SpritemapPane(int default_palette_idx = 0);

    void attach_debug_container(std::shared_ptr<DebugContainer<u16, u8, 16>> debug_container);

    void draw(char const* title, u32 tile_texture, bool* p_open = nullptr);

private:
    static constexpr int width = 256;
    static constexpr int height = 64;
    static constexpr unsigned int sprites_per_row = 16;

    static constexpr float scale = 4.0;
    static constexpr int scaled_width = static_cast<int>(scale * static_cast<float>(width));
    static constexpr int scaled_height = static_cast<int>(scale * static_cast<float>(height));

    std::shared_ptr<DebugContainer<u16, u8, 16>> m_debug_container;
    bool m_is_debug_container_set;
    std::vector<std::vector<Framebuffer>> m_framebuffers;
    int m_chosen_palette_idx;
    int m_number_of_palettes;
    int m_chosen_rotation;
    int m_number_of_rotations;
    ImGuiSliderFlags m_slider_flags;
    bool m_are_all_sprites_rendered;

    void prepare_framebuffers();

    bool prepare_framebuffer(unsigned int palette_idx);

    bool prepare_framebuffer_for_rotation(
        std::vector<std::shared_ptr<Sprite>> const& sprites,
        unsigned int rotation,
        unsigned int palette_idx);

    void render_image(u32 tile_texture);
};
}
