#include "gui.h"
#include "crosscutting/gui/graphics/sprite.h"
#include "crosscutting/gui/graphics/tile.h"
#include "crosscutting/util/gui_util.h"
#include "lcd_control.h"
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
    }
    if (tile_rom.size() % 16 != 0) {
        throw std::runtime_error("Programming error: The size of the tile ROM is not divisible by 16");
    }

    m_tile_rom = tile_rom;

    m_has_loaded_tile_rom = true;
}

void Gui::load_sprite_rom(std::vector<u8> const& sprite_rom)
{
    if (m_has_loaded_sprite_rom) {
        throw std::runtime_error("Programming error: The sprite ROM has already been loaded");
    }
    if (sprite_rom.size() % 64 != 0) {
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

auto Gui::tiles() -> std::vector<std::vector<std::shared_ptr<Tile>>>
{
    return m_tiles;
}

auto Gui::sprites() -> std::tuple<
    std::vector<std::vector<std::shared_ptr<Sprite>>>,
    std::vector<std::vector<std::shared_ptr<Sprite>>>,
    std::vector<std::vector<std::shared_ptr<Sprite>>>,
    std::vector<std::vector<std::shared_ptr<Sprite>>>>
{
    return { m_sprites, m_sprites_x, m_sprites_y, m_sprites_xy };
}

auto Gui::render_tile([[maybe_unused]] u8 palette_idx, [[maybe_unused]] u8 tile_idx) -> std::shared_ptr<Tile>
{
    auto new_tile = std::make_shared<Tile>(s_tile_size, s_tile_size);

    return new_tile;
}

auto Gui::render_debugging_tile([[maybe_unused]] u8 tile_idx) -> std::shared_ptr<Tile>
{
    auto new_tile = std::make_shared<Tile>(s_tile_size, s_tile_size);

    return new_tile;
}

void Gui::render_play_area(
    [[maybe_unused]] Framebuffer& screen,
    [[maybe_unused]] std::vector<u8> const& tile_ram,
    [[maybe_unused]] std::vector<u8> const& palette_ram
)
{
}

void Gui::draw_tiles(
    Framebuffer& screen,
    std::vector<u8> const& tile_ram,
    std::vector<u8> const& palette_ram
)
{
    render_play_area(screen, tile_ram, palette_ram);
}

auto Gui::render_sprite(
    [[maybe_unused]] u8 palette_idx,
    [[maybe_unused]] u8 sprite_idx,
    [[maybe_unused]] bool flip_x,
    [[maybe_unused]] bool flip_y
) -> std::shared_ptr<Sprite>
{
    auto new_sprite = std::make_shared<Sprite>(s_sprite_size, s_sprite_size);

    return new_sprite;
}

auto Gui::render_debugging_sprite([[maybe_unused]] unsigned int rotation, [[maybe_unused]] u8 sprite_idx) -> std::shared_ptr<Sprite>
{
    auto new_sprite = std::make_shared<Sprite>(s_sprite_size, s_sprite_size);
    return new_sprite;
}

void Gui::draw_sprites([[maybe_unused]] Framebuffer& screen, [[maybe_unused]] std::vector<u8> const& sprite_ram)
{
}

auto Gui::create_framebuffer(
    const LcdControl &lcd_control,
    [[maybe_unused]] std::vector<u8> const& tile_ram,
    [[maybe_unused]] std::vector<u8> const& sprite_ram,
    [[maybe_unused]] std::vector<u8> const& palette_ram) -> std::vector<u32>
{
    if (!m_memory_mapper_is_attached) {
        throw std::runtime_error("Programming error: The memory mapper is not attached");
    }

    if (lcd_control.m_is_bg_and_window_enabled) {
        draw_tiles(m_framebuffer, tile_ram, palette_ram);
    }

    if (lcd_control.m_is_obj_enabled) {
        draw_sprites(m_framebuffer, sprite_ram);
    }

    return m_framebuffer.to_output_vector();
}
}
