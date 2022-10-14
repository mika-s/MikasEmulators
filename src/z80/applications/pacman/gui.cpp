#include <iostream>
#include "gui.h"

namespace emu::z80::applications::pacman {

    using emu::gui::UninitializedSprite;
    using emu::gui::UninitializedTile;

    Gui::Gui()
            : m_framebuffer(Framebuffer(height, width, Color(0xff, 0, 128, 255))) {
    }

    void Gui::load_color_rom(const std::vector<u8> &color_rom) {
        if (m_has_loaded_color_rom) {
            throw std::runtime_error("Programming error: The color ROM has already been loaded");
        }

        for (u8 byte: color_rom) {
            const u8 r = (is_bit_set(byte, 0) ? 0x21 : 0)
                         + (is_bit_set(byte, 1) ? 0x47 : 0)
                         + (is_bit_set(byte, 2) ? 0x97 : 0);

            const u8 g = (is_bit_set(byte, 3) ? 0x21 : 0)
                         + (is_bit_set(byte, 4) ? 0x47 : 0)
                         + (is_bit_set(byte, 5) ? 0x97 : 0);

            const u8 b = (is_bit_set(byte, 6) ? 0x51 : 0)
                         + (is_bit_set(byte, 7) ? 0xae : 0);

            m_colors.emplace_back(0xff, r, g, b);
        }

        m_has_loaded_color_rom = true;
    }

    void Gui::load_palette_rom(const std::vector<u8> &palette_rom) {
        if (m_has_loaded_palette_rom) {
            throw std::runtime_error("Programming error: The palette ROM has already been loaded");
        } else if (!m_has_loaded_color_rom) {
            throw std::runtime_error("Programming error: The color ROM has to be loaded before the palette ROM");
        } else if (palette_rom.size() % 4 != 0) {
            throw std::runtime_error("Programming error: The size of the palette ROM is not divisible by 4");
        }

        std::vector<Color> colors;

        for (u8 byte: palette_rom) {
            colors.push_back(m_colors[byte]);

            if (colors.size() == 4) {
                m_palettes.emplace_back(colors[0], colors[1], colors[2], colors[3]);
                colors.clear();
            }
        }

        m_number_of_palettes = m_palettes.size();
        m_has_loaded_palette_rom = true;
    }

    void Gui::load_tile_rom(const std::vector<u8> &tile_rom) {
        if (m_has_loaded_tile_rom) {
            throw std::runtime_error("Programming error: The tile ROM has already been loaded");
        } else if (!m_has_loaded_palette_rom) {
            throw std::runtime_error("Programming error: The palette ROM has to be loaded before the tile ROM");
        } else if (tile_rom.size() % 16 != 0) {
            throw std::runtime_error("Programming error: The size of the tile ROM is not divisible by 16");
        }

        m_tile_rom = tile_rom;

        const int tile_count = tile_rom.size() / bytes_per_tile;

        for (size_t palette_idx = 0; palette_idx < m_palettes.size(); ++palette_idx) {
            m_tiles.emplace_back(tile_count, std::make_shared<UninitializedTile>());

            for (int tile_idx = 0; tile_idx < tile_count; ++tile_idx) {
                m_tiles[palette_idx][tile_idx] = render_tile(palette_idx, tile_idx);
            }
        }

        m_has_loaded_tile_rom = true;
    }

    void Gui::load_sprite_rom(const std::vector<u8> &sprite_rom) {
        if (m_has_loaded_sprite_rom) {
            throw std::runtime_error("Programming error: The sprite ROM has already been loaded");
        } else if (!m_has_loaded_palette_rom) {
            throw std::runtime_error("Programming error: The palette ROM has to be loaded before the tile ROM");
        } else if (sprite_rom.size() % 64 != 0) {
            throw std::runtime_error("Programming error: The size of the sprite ROM is not divisible by 64");
        }

        m_sprite_rom = sprite_rom;

        const unsigned int sprite_count = sprite_rom.size() / bytes_per_sprite;

        for (size_t palette_idx = 0; palette_idx < m_palettes.size(); ++palette_idx) {
            m_sprites.emplace_back(sprite_count, std::make_shared<UninitializedSprite>());
            m_sprites_x.emplace_back(sprite_count, std::make_shared<UninitializedSprite>());
            m_sprites_y.emplace_back(sprite_count, std::make_shared<UninitializedSprite>());
            m_sprites_xy.emplace_back(sprite_count, std::make_shared<UninitializedSprite>());

            for (unsigned int sprite_idx = 0; sprite_idx < sprite_count; ++sprite_idx) {
                m_sprites[palette_idx][sprite_idx] = render_sprite(palette_idx, sprite_idx, false, false);
                m_sprites_x[palette_idx][sprite_idx] = render_sprite(palette_idx, sprite_idx, true, false);
                m_sprites_y[palette_idx][sprite_idx] = render_sprite(palette_idx, sprite_idx, false, true);
                m_sprites_xy[palette_idx][sprite_idx] = render_sprite(palette_idx, sprite_idx, true, true);
            }
        }

        m_has_loaded_sprite_rom = true;
    }

    std::vector<std::vector<std::shared_ptr<Tile>>> Gui::tiles() {
        return m_tiles;
    }

    std::tuple<
            std::vector<std::vector<std::shared_ptr<Sprite>>>,
            std::vector<std::vector<std::shared_ptr<Sprite>>>,
            std::vector<std::vector<std::shared_ptr<Sprite>>>,
            std::vector<std::vector<std::shared_ptr<Sprite>>>
    > Gui::sprites() {
        return {m_sprites, m_sprites_x, m_sprites_y, m_sprites_xy};
    }

    std::shared_ptr<Tile> Gui::render_tile(u8 palette_idx, u8 tile_idx) {
        if (palette_idx >= m_number_of_palettes) {
            palette_idx = 0;
        }

        std::shared_ptr<Tile> tile = m_tiles[palette_idx][tile_idx];

        if (tile->is_initialized())
            return tile;

        const Palette palette = m_palettes[palette_idx];

        std::shared_ptr<Tile> new_tile = std::make_shared<Tile>(tile_size, tile_size);

        const int rom_beginning = tile_idx * bytes_per_tile;
        const int rom_end = rom_beginning + bytes_per_tile;

        int origin_row = 0;
        int origin_col = 0;

        for (int rom_idx = rom_end - 1; rom_idx >= rom_beginning; --rom_idx) {
            const u8 tile_byte = m_tile_rom[rom_idx];

            const int pixel1_color_idx = (is_bit_set(tile_byte, 4) << 1) | is_bit_set(tile_byte, 0);
            const int pixel2_color_idx = (is_bit_set(tile_byte, 5) << 1) | is_bit_set(tile_byte, 1);
            const int pixel3_color_idx = (is_bit_set(tile_byte, 6) << 1) | is_bit_set(tile_byte, 2);
            const int pixel4_color_idx = (is_bit_set(tile_byte, 7) << 1) | is_bit_set(tile_byte, 3);

            new_tile->set(origin_row + 0, origin_col, palette[pixel4_color_idx]);
            new_tile->set(origin_row + 1, origin_col, palette[pixel3_color_idx]);
            new_tile->set(origin_row + 2, origin_col, palette[pixel2_color_idx]);
            new_tile->set(origin_row + 3, origin_col, palette[pixel1_color_idx]);

            if (origin_col == tile_size - 1) {
                origin_row = tile_size / 2;
                origin_col = 0;
            } else {
                ++origin_col;
            }
        }

        return new_tile;
    }

    void Gui::render_play_area(
            Framebuffer &framebuffer,
            const std::vector<u8> &tile_ram,
            const std::vector<u8> &palette_ram
    ) {
        unsigned int origin_row = visible_area_start_row * tile_size;
        unsigned int origin_col = (visible_area_width_in_tiles + border_size_in_tiles - 1) * tile_size;
        unsigned int play_area_row = 0;

        for (int address = playarea_start_address_offset; address <= playarea_stop_address_offset; ++address) {
            const u8 tile_idx = tile_ram[address];
            const u8 palette_idx = palette_ram[address] & 0x7f;

            if (m_is_tile_debug_enabled) {
                render_tile(palette_idx, tile_idx)
                        ->map_debug_overlay_to_framebuffer(framebuffer, origin_row, origin_col, tile_idx);
            } else {
                render_tile(palette_idx, tile_idx)
                        ->map_to_framebuffer(framebuffer, origin_row, origin_col);
            }

            if (play_area_row == play_area_height_in_tiles - 1) {
                origin_col -= tile_size;
                origin_row = visible_area_start_row * tile_size;
                play_area_row = 0;
            } else {
                origin_row += tile_size;
                ++play_area_row;
            }
        }
    }

    void Gui::render_top_bar(
            Framebuffer &framebuffer,
            const std::vector<u8> &tile_ram,
            const std::vector<u8> &palette_ram
    ) {
        // First row
        unsigned int origin_row = 0;
        unsigned int origin_col = 0;

        for (int address = topbar_r1_start_address_offset; topbar_r1_stop_address_offset <= address; --address) {
            const u8 tile_idx = tile_ram[address];
            const u8 palette_idx = palette_ram[address] & 0x7f;

            if (m_is_tile_debug_enabled) {
                render_tile(palette_idx, tile_idx)
                        ->map_debug_overlay_to_framebuffer(framebuffer, origin_row, origin_col, tile_idx);
            } else {
                render_tile(palette_idx, tile_idx)
                        ->map_to_framebuffer(framebuffer, origin_row, origin_col);
            }

            origin_col += tile_size;
        }

        // Second row
        origin_row = tile_size;
        origin_col = 0;

        for (int address = topbar_r2_start_address_offset; topbar_r2_stop_address_offset <= address; --address) {
            const u8 tile_idx = tile_ram[address];
            const u8 palette_idx = palette_ram[address] & 0x7f;

            if (m_is_tile_debug_enabled) {
                render_tile(palette_idx, tile_idx)
                        ->map_debug_overlay_to_framebuffer(framebuffer, origin_row, origin_col, tile_idx);
            } else {
                render_tile(palette_idx, tile_idx)
                        ->map_to_framebuffer(framebuffer, origin_row, origin_col);
            }

            origin_col += tile_size;
        }
    }

    void Gui::render_bottom_bar(
            Framebuffer &framebuffer,
            const std::vector<u8> &tile_ram,
            const std::vector<u8> &palette_ram
    ) {
        // First row
        unsigned int origin_col = 0;
        unsigned int origin_row = bottombar_start_row * tile_size;

        for (int address = bottombar_r1_start_address_offset; bottombar_r1_stop_address_offset <= address; --address) {
            const u8 tile_idx = tile_ram[address];
            const u8 palette_idx = palette_ram[address] & 0x7f;

            if (m_is_tile_debug_enabled) {
                render_tile(palette_idx, tile_idx)
                        ->map_debug_overlay_to_framebuffer(framebuffer, origin_row, origin_col, tile_idx);
            } else {
                render_tile(palette_idx, tile_idx)
                        ->map_to_framebuffer(framebuffer, origin_row, origin_col);
            }

            origin_col += tile_size;
        }

        // Second row
        origin_col = 0;
        origin_row = (bottombar_start_row + 1) * tile_size;

        for (int address = bottombar_r2_start_address_offset; bottombar_r2_stop_address_offset <= address; --address) {
            const u8 tile_idx = tile_ram[address];
            const u8 palette_idx = palette_ram[address] & 0x7f;

            if (m_is_tile_debug_enabled) {
                render_tile(palette_idx, tile_idx)
                        ->map_debug_overlay_to_framebuffer(framebuffer, origin_row, origin_col, tile_idx);
            } else {
                render_tile(palette_idx, tile_idx)
                        ->map_to_framebuffer(framebuffer, origin_row, origin_col);
            }

            origin_col += tile_size;
        }
    }

    /**
     *      0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31
     *
     * 0   3DF 3DE 3DD 3DC 3DB 3DA 3D9 3D8 3D7 3D6 3D5 3D4 3D3 3D2 3D1 3D0 3CF 3CE 3CD 3CC 3CB 3CA 3C9 3C8 3C7 3C6 3C5 3C4 3C3 3C2 3C1 3C0
     * 1   3FF 3FE 3FD 3FC 3FB 3FA 3F9 3F8 3F7 3F6 3F5 3F4 3F3 3F2 3F1 3F0 3EF 3EE 3ED 3EC 3EB 3EA 3E9 3E8 3E7 3E6 3E5 3E4 3E3 3E2 3E1 3E0
     * 2           3A0 380 360 340 320 300 2E0 2C0 2A0 280 260 240 220 200 1E0 1C0 1A0 180 160 140 120 100 0E0 0C0 0A0 080 060 040
     * 3           3A1 381 361 341 321 301 2E1 2C1 2A1 281 261 241 221 201 1E1 1C1 1A1 181 161 141 121 101 0E1 0C1 0A1 081 061 041
     * 4           3A2 382 362 342 322 302 2E2 2C2 2A2 282 262 242 222 202 1E2 1C2 1A2 182 162 142 122 102 0E2 0C2 0A2 082 062 042
     * 5           3A3 383 363 343 323 303 2E3 2C3 2A3 283 263 243 223 203 1E3 1C3 1A3 183 163 143 123 103 0E3 0C3 0A3 083 063 043
     * 6           3A4 384 364 344 324 304 2E4 2C4 2A4 284 264 244 224 204 1E4 1C4 1A4 184 164 144 124 104 0E4 0C4 0A4 084 064 044
     * 7           3A5 385 365 345 325 305 2E5 2C5 2A5 285 265 245 225 205 1E5 1C5 1A5 185 165 145 125 105 0E5 0C5 0A5 085 065 045
     * 8           3A6 386 366 346 326 306 2E6 2C6 2A6 286 266 246 226 206 1E6 1C6 1A6 186 166 146 126 106 0E6 0C6 0A6 086 066 046
     * 9           3A7 387 367 347 327 307 2E7 2C7 2A7 287 267 247 227 207 1E7 1C7 1A7 187 167 147 127 107 0E7 0C7 0A7 087 067 047
     * 10          3A8 388 368 348 328 308 2E8 2C8 2A8 288 268 248 228 208 1E8 1C8 1A8 188 168 148 128 108 0E8 0C8 0A8 088 068 048
     * 11          3A9 389 369 349 329 309 2E9 2C9 2A9 289 269 249 229 209 1E9 1C9 1A9 189 169 149 129 109 0E9 0C9 0A9 089 069 049
     * 12          3AA 38A 36A 34A 32A 30A 2EA 2CA 2AA 28A 26A 24A 22A 20A 1EA 1CA 1AA 18A 16A 14A 12A 10A 0EA 0CA 0AA 08A 06A 04A
     * 13          3AB 38B 36B 34B 32B 30B 2EB 2CB 2AB 28B 26B 24B 22B 20B 1EB 1CB 1AB 18B 16B 14B 12B 10B 0EB 0CB 0AB 08B 06B 04B
     * 14          3AC 38C 36C 34C 32C 30C 2EC 2CC 2AC 28C 26C 24C 22C 20C 1EC 1CC 1AC 18C 16C 14C 12C 10C 0EC 0CC 0AC 08C 06C 04C
     * 15          3AD 38D 36D 34D 32D 30D 2ED 2CD 2AD 28D 26D 24D 22D 20D 1ED 1CD 1AD 18D 16D 14D 12D 10D 0ED 0CD 0AD 08D 06D 04D
     * 16          3AE 38E 36E 34E 32E 30E 2EE 2CE 2AE 28E 26E 24E 22E 20E 1EE 1CE 1AE 18E 16E 14E 12E 10E 0EE 0CE 0AE 08E 06E 04E
     * 17          3AF 38F 36F 34F 32F 30F 2EF 2CF 2AF 28F 26F 24F 22F 20F 1EF 1CF 1AF 18F 16F 14F 12F 10F 0EF 0CF 0AF 08F 06F 04F
     * 18          3B0 390 370 350 330 310 2F0 2D0 2B0 290 270 250 230 210 1F0 1D0 1B0 190 170 150 130 110 0F0 0D0 0B0 090 070 050
     * 19          3B1 391 371 351 331 311 2F1 2D1 2B1 291 271 251 231 211 1F1 1D1 1B1 191 171 151 131 111 0F1 0D1 0B1 091 071 051
     * 20          3B2 392 372 352 332 312 2F2 2D2 2B2 292 272 252 232 212 1F2 1D2 1B2 192 172 152 132 112 0F2 0D2 0B2 092 072 052
     * 21          3B3 393 373 353 333 313 2F3 2D3 2B3 293 273 253 233 213 1F3 1D3 1B3 193 173 153 133 113 0F3 0D3 0B3 093 073 053
     * 22          3B4 394 374 354 334 314 2F4 2D4 2B4 294 274 254 234 214 1F4 1D4 1B4 194 174 154 134 114 0F4 0D4 0B4 094 074 054
     * 23          3B5 395 375 355 335 315 2F5 2D5 2B5 295 275 255 235 215 1F5 1D5 1B5 195 175 155 135 115 0F5 0D5 0B5 095 075 055
     * 24          3B6 396 376 356 336 316 2F6 2D6 2B6 296 276 256 236 216 1F6 1D6 1B6 196 176 156 136 116 0F6 0D6 0B6 096 076 056
     * 25          3B7 397 377 357 337 317 2F7 2D7 2B7 297 277 257 237 217 1F7 1D7 1B7 197 177 157 137 117 0F7 0D7 0B7 097 077 057
     * 26          3B8 398 378 358 338 318 2F8 2D8 2B8 298 278 258 238 218 1F8 1D8 1B8 198 178 158 138 118 0F8 0D8 0B8 098 078 058
     * 27          3B9 399 379 359 339 319 2F9 2D9 2B9 299 279 259 239 219 1F9 1D9 1B9 199 179 159 139 119 0F9 0D9 0B9 099 079 059
     * 28          3BA 39A 37A 35A 33A 31A 2FA 2DA 2BA 29A 27A 25A 23A 21A 1FA 1DA 1BA 19A 17A 15A 13A 11A 0FA 0DA 0BA 09A 07A 05A
     * 29          3BB 39B 37B 35B 33B 31B 2FB 2DB 2BB 29B 27B 25B 23B 21B 1FB 1DB 1BB 19B 17B 15B 13B 11B 0FB 0DB 0BB 09B 07B 05B
     * 30          3BC 39C 37C 35C 33C 31C 2FC 2DC 2BC 29C 27C 25C 23C 21C 1FC 1DC 1BC 19C 17C 15C 13C 11C 0FC 0DC 0BC 09C 07C 05C
     * 31          3BD 39D 37D 35D 33D 31D 2FD 2DD 2BD 29D 27D 25D 23D 21D 1FD 1DD 1BD 19D 17D 15D 13D 11D 0FD 0DD 0BD 09D 07D 05D
     * 32          3BE 39E 37E 35E 33E 31E 2FE 2DE 2BE 29E 27E 25E 23E 21E 1FE 1DE 1BE 19E 17E 15E 13E 11E 0FE 0DE 0BE 09E 07E 05E
     * 33          3BF 39F 37F 35F 33F 31F 2FF 2DF 2BF 29F 27F 25F 23F 21F 1FF 1DF 1BF 19F 17F 15F 13F 11F 0FF 0DF 0BF 09F 07F 05F
     * 34  01F 01E 01D 01C 01B 01A 019 018 017 016 015 014 013 012 011 010 00F 00E 00D 00C 00B 00A 009 008 007 006 005 004 003 002 001 000
     * 35  03F 03E 03D 03C 03B 03A 039 038 037 036 035 034 033 032 031 030 02F 02E 02D 02C 02B 02A 029 028 027 026 025 024 023 022 021 020
     */
    void Gui::draw_tiles(
            Framebuffer &framebuffer,
            const std::vector<u8> &tile_ram,
            const std::vector<u8> &palette_ram
    ) {
        render_play_area(framebuffer, tile_ram, palette_ram);
        render_top_bar(framebuffer, tile_ram, palette_ram);
        render_bottom_bar(framebuffer, tile_ram, palette_ram);
    }

    std::shared_ptr<Sprite> Gui::render_sprite(u8 palette_idx, u8 sprite_idx, bool flip_x, bool flip_y) {
        if (palette_idx >= 64) {
            palette_idx = 0;
        }

        std::shared_ptr<Sprite> sprite;

        if (flip_x && flip_y) {
            sprite = m_sprites_xy[palette_idx][sprite_idx];
        } else if (flip_x) {
            sprite = m_sprites_x[palette_idx][sprite_idx];
        } else if (flip_y) {
            sprite = m_sprites_y[palette_idx][sprite_idx];
        } else {
            sprite = m_sprites[palette_idx][sprite_idx];
        }

        if (sprite->is_initialized()) {
            return sprite;
        }

        const Palette palette = m_palettes[palette_idx];

        std::shared_ptr<Sprite> new_sprite = std::make_shared<Sprite>(sprite_size, sprite_size);

        int origin_row = 0;
        int origin_col = 0;

        const std::vector<int> group_idx_order = {5, 1, 6, 2, 7, 3, 4, 0};
        for (int group_idx: group_idx_order) {
            const int beginning = (sprite_idx * bytes_per_sprite) + (group_idx * 8);
            const int end = (sprite_idx * bytes_per_sprite) + (group_idx * 8) + 8;

            for (int rom_idx = end - 1; beginning <= rom_idx; --rom_idx) {
                const u8 sprite_byte = m_sprite_rom[rom_idx];

                const int pixel1_color_idx = (is_bit_set(sprite_byte, 4) << 1) | is_bit_set(sprite_byte, 0);
                const int pixel2_color_idx = (is_bit_set(sprite_byte, 5) << 1) | is_bit_set(sprite_byte, 1);
                const int pixel3_color_idx = (is_bit_set(sprite_byte, 6) << 1) | is_bit_set(sprite_byte, 2);
                const int pixel4_color_idx = (is_bit_set(sprite_byte, 7) << 1) | is_bit_set(sprite_byte, 3);

                new_sprite->set(origin_row + 0, origin_col, palette[pixel4_color_idx]);
                new_sprite->set(origin_row + 1, origin_col, palette[pixel3_color_idx]);
                new_sprite->set(origin_row + 2, origin_col, palette[pixel2_color_idx]);
                new_sprite->set(origin_row + 3, origin_col, palette[pixel1_color_idx]);

                if (origin_col == sprite_size - 1) {
                    origin_col = 0;
                    origin_row += 4;
                } else {
                    ++origin_col;
                }
            }
        }

        if (flip_x) {
            new_sprite->flip_horizontal();
        }

        if (flip_y) {
            new_sprite->flip_vertical();
        }

        return new_sprite;
    }

    void Gui::draw_sprites(Framebuffer &framebuffer, const std::vector<u8> &sprite_ram) {
        u16 sprite_coordinates_address = 0x506f - sprite_ram_address_offset;
        u16 sprite_data_address = 0x4fff - sprite_ram_address_offset;

        for (int sprite_no = 0; sprite_no < number_of_sprites; ++sprite_no) {
            const u8 palette_idx = sprite_ram[sprite_data_address--];
            const u8 flags = sprite_ram[sprite_data_address--];

            const bool flip_y = is_bit_set(flags, 1);
            const bool flip_x = is_bit_set(flags, 2);
            const u8 sprite_idx = (flags & 0b11111100) >> 2;

            std::shared_ptr<Sprite> sprite = render_sprite(palette_idx, sprite_idx, flip_x, flip_y);

            const int sprite_origin_row = sprite_ram[sprite_coordinates_address--];
            const int sprite_origin_col = sprite_ram[sprite_coordinates_address--];

            const int converted_row = height - (border_size_in_tiles * tile_size) - sprite_origin_row;
            const int converted_col = width - sprite_origin_col - 1;

            sprite->map_to_framebuffer(framebuffer, converted_row, converted_col);
        }
    }

    void Gui::draw_edges(Framebuffer &framebuffer) {
        for (int row = 0; row < height; row++) {
            for (int col = 0; col < width_invisible_border; col++) {
                framebuffer.set(row, col, Color::black());
            }
        }

        for (int row = 0; row < height; row++) {
            for (int col = width - width_invisible_border; col < width; col++) {
                framebuffer.set(row, col, Color::black());
            }
        }
    }

    std::vector<u32> Gui::create_framebuffer(
            const std::vector<u8> &tile_ram,
            const std::vector<u8> &sprite_ram,
            const std::vector<u8> &palette_ram
    ) {
        if (!m_has_loaded_color_rom) {
            throw std::runtime_error("Programming error: The color ROM has not been loaded");
        } else if (!m_has_loaded_palette_rom) {
            throw std::runtime_error("Programming error: The palette ROM has not been loaded");
        } else if (!m_has_loaded_tile_rom) {
            throw std::runtime_error("Programming error: The tile ROM has not been loaded");
        } else if (!m_has_loaded_sprite_rom) {
            throw std::runtime_error("Programming error: The sprite ROM has not been loaded");
        }

        draw_tiles(m_framebuffer, tile_ram, palette_ram);
        draw_sprites(m_framebuffer, sprite_ram);
        draw_edges(m_framebuffer);

        return m_framebuffer.to_output_vector();
    }
}
