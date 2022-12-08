#include "spritemap_pane.h"
#include "crosscutting/typedefs.h"
#include "debugging/debug_container.h"
#include "glad/glad.h"
#include "gui/graphics/color.h"
#include "gui/graphics/framebuffer.h"
#include "gui/graphics/sprite.h"
#include <bits/utility.h>
#include <cstddef>
#include <cstdint>
#include <ext/alloc_traits.h>
#include <stdexcept>
#include <tuple>
#include <utility>

namespace emu::gui {

SpritemapPane::SpritemapPane(int default_palette_idx)
    : m_is_debug_container_set(false)
    , m_framebuffers({ {}, {}, {}, {} })
    , m_chosen_palette_idx(default_palette_idx)
    , m_number_of_palettes(0)
    , m_chosen_rotation(0)
    , m_number_of_rotations(4)
    , m_slider_flags(ImGuiSliderFlags_AlwaysClamp)
    , m_are_all_sprites_rendered(false)
{
    if (default_palette_idx < 0) {
        throw std::invalid_argument("default_palette_idx cannot be negative");
    }
}

void SpritemapPane::attach_debug_container(std::shared_ptr<DebugContainer<u16, u8, 16>> debug_container)
{
    m_debug_container = std::move(debug_container);
    m_is_debug_container_set = true;
}

void SpritemapPane::draw(char const* title, u32 sprite_texture, bool* p_open)
{
    if (!ImGui::Begin(title, p_open, ImGuiWindowFlags_MenuBar)) {
        ImGui::End();
        return;
    }

    if (!m_is_debug_container_set) {
        ImGui::Text("The debug container is not provided this pane.");
    } else if (!m_debug_container->is_spritemap_set()) {
        ImGui::Text("The spritemap is not provided to this pane.");
    } else {
        if (m_are_all_sprites_rendered) {
            ImGui::SliderInt(
                "Palette index",
                &m_chosen_palette_idx,
                0,
                m_number_of_palettes - 1,
                "%d",
                m_slider_flags);
            ImGui::SliderInt(
                "Rotation",
                &m_chosen_rotation,
                0,
                m_number_of_rotations - 1,
                "%d",
                m_slider_flags);
            ImGui::BeginChild("spritemap_image_child", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
            render_image(sprite_texture);
            ImGui::EndChild();
        } else {
            prepare_framebuffers();
        }
    }

    ImGui::End();
}

void SpritemapPane::prepare_framebuffers()
{
    const std::size_t number_of_palettes = std::get<0>(m_debug_container->sprites()).size();

    for (std::size_t palette_idx = 0; palette_idx < number_of_palettes; ++palette_idx) {
        if (!prepare_framebuffer(palette_idx)) {
            return;
        }
    }

    m_are_all_sprites_rendered = true;
    m_number_of_palettes = number_of_palettes;
}

bool SpritemapPane::prepare_framebuffer(unsigned int palette_idx)
{
    auto const all_sprites = m_debug_container->sprites();

    const std::vector<std::shared_ptr<Sprite>> sprites = std::get<0>(all_sprites)[palette_idx];
    if (!prepare_framebuffer_for_rotation(sprites, 0, palette_idx)) {
        return false;
    }

    const std::vector<std::shared_ptr<Sprite>> sprites_x = std::get<1>(all_sprites)[palette_idx];
    if (!prepare_framebuffer_for_rotation(sprites_x, 1, palette_idx)) {
        return false;
    }

    const std::vector<std::shared_ptr<Sprite>> sprites_y = std::get<2>(all_sprites)[palette_idx];
    if (!prepare_framebuffer_for_rotation(sprites_y, 2, palette_idx)) {
        return false;
    }

    const std::vector<std::shared_ptr<Sprite>> sprites_xy = std::get<3>(all_sprites)[palette_idx];
    if (!prepare_framebuffer_for_rotation(sprites_xy, 3, palette_idx)) {
        return false;
    }

    return true;
}

bool SpritemapPane::prepare_framebuffer_for_rotation(
    std::vector<std::shared_ptr<Sprite>> const& sprites,
    unsigned int rotation,
    unsigned int palette_idx)
{
    const std::size_t number_of_sprites = sprites.size();
    const std::size_t sprite_size = sprites[0]->size();
    const std::size_t rows = number_of_sprites / sprites_per_row;

    for (unsigned int row = 0; row < rows; ++row) {
        for (unsigned int col = 0; col < sprites_per_row; ++col) {
            std::shared_ptr<Sprite> const& sprite = sprites[row * sprites_per_row + col];

            if (!sprite->is_initialized()) {
                return false;
            }

            if (m_framebuffers[rotation].size() <= palette_idx) {
                m_framebuffers[rotation].emplace_back(height, width, Color::black());
            }

            sprite->map_to_framebuffer(m_framebuffers[rotation][palette_idx], row * sprite_size, col * sprite_size);
        }
    }

    return true;
}

void SpritemapPane::render_image(u32 tile_texture)
{
    glBindTexture(GL_TEXTURE_2D, tile_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, //
        m_framebuffers[m_chosen_rotation][m_chosen_palette_idx].to_output_vector().data());
    glBindTexture(GL_TEXTURE_2D, 0);

    const ImVec2 image_size = ImVec2(scaled_width, scaled_height);
    ImGui::Image(
        (void*)((intptr_t)tile_texture), image_size,
        ImVec2(0, 0),
        ImVec2(1, 1),
        ImColor(255, 255, 255, 255),
        ImColor(0, 0, 0, 0));
}
}
