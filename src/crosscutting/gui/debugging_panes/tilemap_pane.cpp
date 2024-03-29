#include "tilemap_pane.h"
#include "crosscutting/typedefs.h"
#include "debugging/debug_container.h"
#include "glad/glad.h"
#include "gui/graphics/color.h"
#include "gui/graphics/framebuffer.h"
#include "gui/graphics/tile.h"
#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <utility>

namespace emu::gui {

TilemapPane::TilemapPane(int default_palette_idx)
    : m_is_debug_container_set(false)
    , m_framebuffers({})
    , m_chosen_palette_idx(default_palette_idx)
    , m_number_of_palettes(0)
    , m_slider_flags(ImGuiSliderFlags_AlwaysClamp)
    , m_are_all_tiles_rendered(false)
{
    if (default_palette_idx < 0) {
        throw std::invalid_argument("default_palette_idx cannot be negative");
    }
}

void TilemapPane::attach_debug_container(std::shared_ptr<DebugContainer<u16, u8, 16>> debug_container)
{
    m_debug_container = std::move(debug_container);
    m_is_debug_container_set = true;
}

void TilemapPane::draw(char const* title, u32 tile_texture, bool* p_open)
{
    if (!ImGui::Begin(title, p_open, ImGuiWindowFlags_MenuBar)) {
        ImGui::End();
        return;
    }

    if (!m_is_debug_container_set) {
        ImGui::Text("The debug container is not provided this pane.");
    } else if (!m_debug_container->is_tilemap_set()) {
        ImGui::Text("The tilemap is not provided to this pane.");
    } else {
        if (m_are_all_tiles_rendered) {
            ImGui::SliderInt(
                "Palette index",
                &m_chosen_palette_idx,
                0,
                m_number_of_palettes - 1,
                "%d",
                m_slider_flags);
            render_image(tile_texture);
        } else {
            prepare_framebuffers();
        }
    }

    ImGui::End();
}

void TilemapPane::prepare_framebuffers()
{
    const std::size_t number_of_palettes = m_debug_container->tiles().size();

    for (std::size_t palette_idx = 0; palette_idx < number_of_palettes; ++palette_idx) {
        if (!prepare_framebuffer(palette_idx)) {
            return;
        }
    }

    m_are_all_tiles_rendered = true;
    m_number_of_palettes = number_of_palettes;
}

bool TilemapPane::prepare_framebuffer(unsigned int palette_idx)
{
    const std::vector<std::shared_ptr<Tile>> tiles = m_debug_container->tiles()[palette_idx];
    const std::size_t number_of_tiles = tiles.size();
    const std::size_t tile_size = tiles[0]->size();
    const std::size_t rows = number_of_tiles / tiles_per_row;

    for (unsigned int row = 0; row < rows; ++row) {
        for (unsigned int col = 0; col < tiles_per_row; ++col) {
            std::shared_ptr<Tile> const& tile = tiles[row * tiles_per_row + col];

            if (!tile->is_initialized()) {
                return false;
            }

            if (m_framebuffers.size() <= palette_idx) {
                m_framebuffers.emplace_back(height, width, Color(0xff, 128, 128, 0));
            }

            tile->map_to_framebuffer(m_framebuffers[palette_idx], row * tile_size, col * tile_size);
        }
    }

    return true;
}

void TilemapPane::render_image(u32 tile_texture)
{
    glBindTexture(GL_TEXTURE_2D, tile_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, //
        m_framebuffers[m_chosen_palette_idx].to_output_vector().data());
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
