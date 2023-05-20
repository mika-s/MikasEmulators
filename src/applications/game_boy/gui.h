#pragma once

#include "crosscutting/gui/graphics/color.h"
#include "crosscutting/gui/graphics/framebuffer.h"
#include "crosscutting/gui/graphics/palette.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include <cstddef>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

namespace emu::applications::game_boy {
class GuiObserver;
class MemoryMappedIoForGameBoy;
}
namespace emu::debugger {
template<class A, class D, std::size_t B>
class DebugContainer;
}
namespace emu::debugger {
template<class A, std::size_t B>
class Debugger;
}
namespace emu::gui {
class Sprite;
}
namespace emu::gui {
class Tile;
}
namespace emu::logging {
class Logger;
}

namespace emu::applications::game_boy {

using emu::debugger::DebugContainer;
using emu::debugger::Debugger;
using emu::gui::Color;
using emu::gui::Framebuffer;
using emu::gui::Palette;
using emu::gui::Sprite;
using emu::gui::Tile;
using emu::logging::Logger;
using emu::util::byte::is_bit_set;
using emu::util::byte::to_u32;

class Gui {
public:
    Gui();

    virtual ~Gui() = default;

    virtual void add_gui_observer(GuiObserver& observer) = 0;

    virtual void remove_gui_observer(GuiObserver* observer) = 0;

    virtual void update_screen(
        std::vector<u8> const& tile_ram_block_1,
        std::vector<u8> const& tile_ram_block_2,
        std::vector<u8> const& tile_ram_block_3,
        std::vector<u8> const& tile_map_1,
        std::vector<u8> const& tile_map_2,
        std::vector<u8> const& sprite_ram,
        std::vector<u8> const& palette_ram,
        std::string const& game_window_subtitle)
        = 0;

    virtual void update_debug_only() = 0;

    virtual void attach_debugger(std::shared_ptr<Debugger<u16, 16>> debugger) = 0;

    virtual void attach_debug_container(std::shared_ptr<DebugContainer<u16, u8, 16>> debug_container) = 0;

    virtual void attach_logger(std::shared_ptr<Logger> logger) = 0;

    virtual void toggle_tile_debug() = 0;

    virtual void toggle_sprite_debug() = 0;

    void load_tile_rom(std::vector<u8> const& tile_rom);

    void load_sprite_rom(std::vector<u8> const& sprite_rom);

    void attach_memory_mapper(std::shared_ptr<MemoryMappedIoForGameBoy> memory_mapper);

    std::vector<std::vector<std::shared_ptr<Tile>>> tiles();

    std::tuple<
        std::vector<std::vector<std::shared_ptr<Sprite>>>,
        std::vector<std::vector<std::shared_ptr<Sprite>>>,
        std::vector<std::vector<std::shared_ptr<Sprite>>>,
        std::vector<std::vector<std::shared_ptr<Sprite>>>>
    sprites();

protected:
    static constexpr int s_tile_size = 8;
    static constexpr int s_bytes_per_tile = 16;

    static constexpr int s_sprite_size = 16;
    static constexpr int s_bytes_per_sprite = 64;
    static constexpr int s_number_of_sprites = 8;
    static constexpr u16 s_sprite_ram_address_offset = 0x4ff0;

    // Debugging
    static constexpr unsigned int s_tile_offset_row_high_number = 0;
    static constexpr unsigned int s_tile_offset_col_high_number = 1;
    static constexpr unsigned int s_tile_offset_row_low_number = 0;
    static constexpr unsigned int s_tile_offset_col_low_number = 4;
    static constexpr unsigned int s_sprite_offset_row_high_number = 0;
    static constexpr unsigned int s_sprite_offset_col_high_number = 3;
    static constexpr unsigned int s_sprite_offset_row_low_number = 0;
    static constexpr unsigned int s_sprite_offset_col_low_number = 7;
    static constexpr unsigned int s_sprite_offset_row_rotation_number = 9;
    static constexpr unsigned int s_sprite_offset_col_rotation_number = 5;
    static constexpr unsigned int s_sprite_number_of_rotations = 4;

    // Visible area
    static constexpr u16 s_tile_set1_part1_address_offset = 0x8000;
    static constexpr u16 s_tile_set1_part2_address_offset = 0x8800;
    static constexpr u16 s_tile_set2_part1_address_offset = 0x9000;
    static constexpr u16 s_tile_set2_part2_address_offset = 0x9000;
    static constexpr int s_visible_area_width_in_tiles = 20;
    static constexpr int s_width_visible_area = s_visible_area_width_in_tiles * s_tile_size;
    static constexpr int s_visible_area_height_in_tiles = 18;
    static constexpr int s_height_visible_area = s_visible_area_height_in_tiles * s_tile_size;

    // Final width and height
    // Resolution: 160x144 (20x18 tiles)
    static constexpr int s_width = s_width_visible_area;
    static constexpr int s_height = s_height_visible_area;

    static constexpr float s_scale = 4.0;
    static constexpr int s_scaled_width = static_cast<int>(s_scale * static_cast<float>(s_width));
    static constexpr int s_scaled_height = static_cast<int>(s_scale * static_cast<float>(s_height));

    bool m_has_loaded_tile_rom = false;
    bool m_has_loaded_sprite_rom = false;

    bool m_is_tile_debug_enabled = false;
    bool m_is_sprite_debug_enabled = false;

    Framebuffer m_framebuffer;

    std::vector<Color> m_colors;
    std::vector<Palette> m_palettes;
    std::vector<u8> m_tile_rom;
    std::vector<std::vector<std::shared_ptr<Tile>>> m_tiles;
    std::vector<std::shared_ptr<Tile>> m_debugging_tiles;
    std::vector<u8> m_sprite_rom;
    std::vector<std::vector<std::shared_ptr<Sprite>>> m_sprites;
    std::vector<std::vector<std::shared_ptr<Sprite>>> m_sprites_x;
    std::vector<std::vector<std::shared_ptr<Sprite>>> m_sprites_y;
    std::vector<std::vector<std::shared_ptr<Sprite>>> m_sprites_xy;
    std::vector<std::vector<std::shared_ptr<Sprite>>> m_debugging_sprites;

    std::shared_ptr<MemoryMappedIoForGameBoy> m_memory_mapper;
    bool m_memory_mapper_is_attached { false };

    unsigned int m_number_of_palettes;

    void render_play_area(
        Framebuffer& screen,
        std::vector<u8> const& tile_ram,
        std::vector<u8> const& palette_ram);

    std::shared_ptr<Tile> render_tile(u8 palette_idx, u8 tile_idx);

    std::shared_ptr<Tile> render_debugging_tile(u8 tile_idx);

    void draw_tiles(Framebuffer& screen, std::vector<u8> const& tile_ram, std::vector<u8> const& palette_ram);

    std::shared_ptr<Sprite> render_sprite(u8 palette_idx, u8 sprite_idx, bool flip_x, bool flip_y);

    std::shared_ptr<Sprite> render_debugging_sprite(unsigned int rotation, u8 sprite_idx);

    void draw_sprites(Framebuffer& screen, std::vector<u8> const& sprite_ram);

    std::vector<u32> create_framebuffer(
        std::vector<u8> const& tile_ram,
        std::vector<u8> const& sprite_ram,
        std::vector<u8> const& palette_ram);
};
}
