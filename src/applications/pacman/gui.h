#ifndef MIKA_EMULATORS_APPLICATIONS_PACMAN_GUI_H
#define MIKA_EMULATORS_APPLICATIONS_PACMAN_GUI_H

#include "crosscutting/gui/graphics/color.h"
#include "crosscutting/gui/graphics/framebuffer.h"
#include "crosscutting/gui/graphics/palette.h"
#include "crosscutting/misc/run_status.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include <memory>
#include <tuple>
#include <vector>

namespace emu::debugger {
    class DebugContainer;
}
namespace emu::debugger {
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
namespace emu::z80 {
    class GuiObserver;
}

namespace emu::applications::pacman {

    using emu::debugger::DebugContainer;
    using emu::debugger::Debugger;
    using emu::gui::Color;
    using emu::gui::Framebuffer;
    using emu::gui::Palette;
    using emu::gui::Sprite;
    using emu::gui::Tile;
    using emu::logging::Logger;
    using emu::misc::RunStatus;
    using emu::util::byte::is_bit_set;
    using emu::util::byte::to_u32;
    using emu::z80::GuiObserver;

    class Gui {
    public:
        Gui();

        virtual ~Gui() = default;

        virtual void add_gui_observer(GuiObserver &observer) = 0;

        virtual void remove_gui_observer(GuiObserver *observer) = 0;

        virtual void update_screen(
                const std::vector<u8> &tile_ram,
                const std::vector<u8> &sprite_ram,
                const std::vector<u8> &palette_ram,
                RunStatus run_status,
                bool is_screen_flipped
        ) = 0;

        virtual void update_debug_only() = 0;

        virtual void attach_debugger(std::shared_ptr<Debugger> debugger) = 0;

        virtual void attach_debug_container(DebugContainer &debug_container) = 0;

        virtual void attach_logger(std::shared_ptr<Logger> logger) = 0;

        virtual void toggle_tile_debug() = 0;

        virtual void toggle_sprite_debug() = 0;

        void load_color_rom(const std::vector<u8> &color_rom);

        void load_palette_rom(const std::vector<u8> &palette_rom);

        void load_tile_rom(const std::vector<u8> &tile_rom);

        void load_sprite_rom(const std::vector<u8> &sprite_rom);

        std::vector<std::vector<std::shared_ptr<Tile>>> tiles();

        std::tuple<
                std::vector<std::vector<std::shared_ptr<Sprite>>>,
                std::vector<std::vector<std::shared_ptr<Sprite>>>,
                std::vector<std::vector<std::shared_ptr<Sprite>>>,
                std::vector<std::vector<std::shared_ptr<Sprite>>>
        > sprites();

    protected:
        static constexpr int tile_size = 8;
        static constexpr int bytes_per_tile = 16;

        static constexpr int sprite_size = 16;
        static constexpr int bytes_per_sprite = 64;
        static constexpr int number_of_sprites = 8;
        static constexpr u16 sprite_ram_address_offset = 0x4ff0;

        // Debugging
        static constexpr unsigned int tile_offset_row_high_number = 0;
        static constexpr unsigned int tile_offset_col_high_number = 1;
        static constexpr unsigned int tile_offset_row_low_number = 0;
        static constexpr unsigned int tile_offset_col_low_number = 4;
        static constexpr unsigned int sprite_offset_row_high_number = 0;
        static constexpr unsigned int sprite_offset_col_high_number = 3;
        static constexpr unsigned int sprite_offset_row_low_number = 0;
        static constexpr unsigned int sprite_offset_col_low_number = 7;
        static constexpr unsigned int sprite_offset_row_rotation_number = 9;
        static constexpr unsigned int sprite_offset_col_rotation_number = 5;
        static constexpr unsigned int sprite_number_of_rotations = 4;

        // Border
        static constexpr int border_size_in_tiles = 2;
        static constexpr int width_invisible_border = border_size_in_tiles * tile_size;
        static constexpr int width_both_borders = 2 * width_invisible_border;

        // Visible area
        static constexpr u16 playarea_start_address_offset = 0x0040;
        static constexpr u16 playarea_stop_address_offset = 0x03bf;
        static constexpr u16 topbar_r1_start_address_offset = 0x03df;
        static constexpr u16 topbar_r1_stop_address_offset = 0x03c0;
        static constexpr u16 topbar_r2_start_address_offset = 0x03ff;
        static constexpr u16 topbar_r2_stop_address_offset = 0x03e0;
        static constexpr u16 bottombar_r1_start_address_offset = 0x001f;
        static constexpr u16 bottombar_r1_stop_address_offset = 0x0000;
        static constexpr u16 bottombar_r2_start_address_offset = 0x003f;
        static constexpr u16 bottombar_r2_stop_address_offset = 0x0020;
        static constexpr int visible_area_start_row = 2;
        static constexpr int visible_area_width_in_tiles = 28;
        static constexpr int width_visible_area = visible_area_width_in_tiles * tile_size;
        static constexpr int visible_area_height_in_tiles = 36;
        static constexpr int height_visible_area = visible_area_height_in_tiles * tile_size;
        static constexpr int bottombar_start_row = 34;
        static constexpr int play_area_height_in_tiles = 32;

        // Final width and height
        static constexpr int width = width_visible_area + width_both_borders;
        static constexpr int height = height_visible_area;

        static constexpr float scale = 4.0;
        static constexpr int scaled_width = static_cast<int>(scale * static_cast<float>(width));
        static constexpr int scaled_height = static_cast<int>(scale * static_cast<float>(height));

        bool m_has_loaded_color_rom = false;
        bool m_has_loaded_palette_rom = false;
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

        unsigned int m_number_of_palettes;

        void render_play_area(
                Framebuffer &screen,
                const std::vector<u8> &tile_ram,
                const std::vector<u8> &palette_ram
        );

        void render_top_bar(
                Framebuffer &screen,
                const std::vector<u8> &tile_ram,
                const std::vector<u8> &palette_ram
        );

        void render_bottom_bar(
                Framebuffer &screen,
                const std::vector<u8> &tile_ram,
                const std::vector<u8> &palette_ram
        );

        std::shared_ptr<Tile> render_tile(u8 palette_idx, u8 tile_idx);

        std::shared_ptr<Tile> render_debugging_tile(u8 tile_idx);

        void draw_tiles(Framebuffer &screen, const std::vector<u8> &tile_ram, const std::vector<u8> &palette_ram);

        std::shared_ptr<Sprite> render_sprite(u8 palette_idx, u8 sprite_idx, bool flip_x, bool flip_y);

        std::shared_ptr<Sprite> render_debugging_sprite(unsigned int rotation, u8 sprite_idx);

        void draw_sprites(Framebuffer &screen, const std::vector<u8> &sprite_ram);

        static void draw_edges(Framebuffer &screen);

        std::vector<u32> create_framebuffer(
                const std::vector<u8> &tile_ram,
                const std::vector<u8> &sprite_ram,
                const std::vector<u8> &palette_ram,
                bool is_screen_flipped
        );
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_PACMAN_GUI_H
