#include "gui.h"
#include "crosscutting/gui/graphics/sprite.h"
#include "crosscutting/gui/graphics/tile.h"
#include "crosscutting/util/gui_util.h"
#include <stdexcept>
#include <utility>

namespace emu::applications::game_boy {
class MemoryMappedIoForGameBoy;
}

namespace emu::applications::game_boy {

using emu::gui::UninitializedSprite;
using emu::gui::UninitializedTile;
using emu::util::gui::number_to_pixels;

Gui::Gui()
    : m_framebuffer(Framebuffer(s_height, s_width, Color::white()))
    , m_debugging_sprites({ {}, {}, {}, {} })
{
}

void Gui::load_tile_rom(std::vector<u8> const& tile_rom)
{
    if (m_has_loaded_tile_rom) {
        throw std::runtime_error("Programming error: The tile ROM has already been loaded");
    } else if (tile_rom.size() % 16 != 0) {
        throw std::runtime_error("Programming error: The size of the tile ROM is not divisible by 16");
    }

    m_tile_rom = tile_rom;

    m_has_loaded_tile_rom = true;
}

void Gui::load_sprite_rom(std::vector<u8> const& sprite_rom)
{
    if (m_has_loaded_sprite_rom) {
        throw std::runtime_error("Programming error: The sprite ROM has already been loaded");
    } else if (sprite_rom.size() % 64 != 0) {
        throw std::runtime_error("Programming error: The size of the sprite ROM is not divisible by 64");
    }

    m_sprite_rom = sprite_rom;

    m_has_loaded_sprite_rom = true;
}

void Gui::attach_memory_mapper(std::shared_ptr<MemoryMappedIoForGameBoy> memory_mapper)
{
    m_memory_mapper = std::move(memory_mapper);
    m_memory_mapper_is_attached = true;
}

std::vector<std::vector<std::shared_ptr<Tile>>> Gui::tiles()
{
    return m_tiles;
}

std::tuple<
    std::vector<std::vector<std::shared_ptr<Sprite>>>,
    std::vector<std::vector<std::shared_ptr<Sprite>>>,
    std::vector<std::vector<std::shared_ptr<Sprite>>>,
    std::vector<std::vector<std::shared_ptr<Sprite>>>>
Gui::sprites()
{
    return { m_sprites, m_sprites_x, m_sprites_y, m_sprites_xy };
}

std::shared_ptr<Tile> Gui::render_tile([[maybe_unused]] u8 palette_idx, [[maybe_unused]] u8 tile_idx)
{
    std::shared_ptr<Tile> new_tile = std::make_shared<Tile>(s_tile_size, s_tile_size);

    return new_tile;
}

std::shared_ptr<Tile> Gui::render_debugging_tile([[maybe_unused]] u8 tile_idx)
{
    std::shared_ptr<Tile> new_tile = std::make_shared<Tile>(s_tile_size, s_tile_size);

    return new_tile;
}

void Gui::render_play_area(
    [[maybe_unused]] Framebuffer& framebuffer,
    [[maybe_unused]] std::vector<u8> const& tile_ram,
    [[maybe_unused]] std::vector<u8> const& palette_ram)
{
}

void Gui::draw_tiles(
    Framebuffer& framebuffer,
    std::vector<u8> const& tile_ram,
    std::vector<u8> const& palette_ram)
{
    render_play_area(framebuffer, tile_ram, palette_ram);
}

std::shared_ptr<Sprite> Gui::render_sprite([[maybe_unused]] u8 palette_idx, [[maybe_unused]] u8 sprite_idx, [[maybe_unused]] bool flip_x, [[maybe_unused]] bool flip_y)
{
    std::shared_ptr<Sprite> new_sprite = std::make_shared<Sprite>(s_sprite_size, s_sprite_size);

    return new_sprite;
}

std::shared_ptr<Sprite> Gui::render_debugging_sprite([[maybe_unused]] unsigned int rotation, [[maybe_unused]] u8 sprite_idx)
{
    std::shared_ptr<Sprite> new_sprite = std::make_shared<Sprite>(s_sprite_size, s_sprite_size);
    return new_sprite;
}

void Gui::draw_sprites([[maybe_unused]] Framebuffer& framebuffer, [[maybe_unused]] std::vector<u8> const& sprite_ram)
{
}

std::vector<u32> Gui::create_framebuffer(
    [[maybe_unused]] std::vector<u8> const& tile_ram,
    [[maybe_unused]] std::vector<u8> const& sprite_ram,
    [[maybe_unused]] std::vector<u8> const& palette_ram)
{
    if (!m_memory_mapper_is_attached) {
        throw std::runtime_error("Programming error: The memory mapper is not attached");
    }
    // else if (!m_has_loaded_palette_rom) {
    //        throw std::runtime_error("Programming error: The palette ROM has not been loaded");
    //    } else if (!m_has_loaded_tile_rom) {
    //        throw std::runtime_error("Programming error: The tile ROM has not been loaded");
    //    } else if (!m_has_loaded_sprite_rom) {
    //        throw std::runtime_error("Programming error: The sprite ROM has not been loaded");
    //    }
    //
    //    bool const is_not_debugging_tiles_or_sprites = !(m_is_tile_debug_enabled || m_is_sprite_debug_enabled);
    //    bool const should_draw_tiles = m_is_tile_debug_enabled || is_not_debugging_tiles_or_sprites;
    //    bool const should_draw_sprites = m_is_sprite_debug_enabled || is_not_debugging_tiles_or_sprites;
    //
    //    if (should_draw_tiles) {
    //        draw_tiles(m_framebuffer, tile_ram, palette_ram);
    //    } else {
    //        m_framebuffer.clear();
    //    }
    //    if (should_draw_sprites) {
    //        draw_sprites(m_framebuffer, sprite_ram);
    //    }

    return m_framebuffer.to_output_vector();
}
}
