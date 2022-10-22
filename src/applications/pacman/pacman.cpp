#include "pacman.h"
#include "crosscutting/util/file_util.h"

namespace emu::applications::pacman {

    using emu::z80::RunStatus;
    using emu::util::file::read_file_into_vector;

    Pacman::Pacman(
            const Settings &settings,
            std::shared_ptr<Gui> gui,
            std::shared_ptr<Input> input
    )
            : m_gui(std::move(gui)),
              m_input(std::move(input)) {
        load_file();

        m_memory_mapped_io = std::make_shared<MemoryMappedIo>();
        m_memory_mapped_io->dipswitches(settings);
        m_memory_mapped_io->board_test(settings);
    }

    std::unique_ptr<Session> Pacman::new_session() {
        return std::make_unique<PacmanSession>(
                m_gui,
                m_input,
                m_memory_mapped_io,
                m_memory
        );
    }

    void Pacman::load_file() {
        const std::string directory = "roms/z80/pacman/";
        m_memory.add(read_file_into_vector(directory + "pacman.6e")); // $0000-$0fff: pacman.6e, code
        m_memory.add(read_file_into_vector(directory + "pacman.6f")); // $1000-$1fff: pacman.6f, code
        m_memory.add(read_file_into_vector(directory + "pacman.6h")); // $2000-$2fff: pacman.6h, code
        m_memory.add(read_file_into_vector(directory + "pacman.6j")); // $3000-$3fff: pacman.6j, code
        m_memory.add(create_tile_ram());                              // $4000-$43ff: tile RAM
        m_memory.add(create_palette_ram());                           // $4400-$47ff: palette RAM
        m_memory.add(create_ram());                                   // $4800-$4fef: RAM
        m_memory.add(create_sprite_ram());                            // $4ff0-$4fff: sprite RAM
        m_memory.add(create_memory_mapped_io());                      // $5000-$50ff: memory-mapped IO
        m_memory.add(fill_remaining(m_memory.size()));
        m_memory.add_address_mask(address_mask);


        m_color_rom.add(read_file_into_vector(directory + "82s123.7f"));   // $0000-$0020: 82s123.7f, colors
        m_palette_rom.add(read_file_into_vector(directory + "82s126.4a")); // $0000-$00ff: 82s126.4a, palettes
        m_tile_rom.add(read_file_into_vector(directory + "pacman.5e"));    // $0000-$0fff: pacman.5e, tiles
        m_sprite_rom.add(read_file_into_vector(directory + "pacman.5f"));  // $0000-$0fff: pacman.5f, sprites

        m_gui->load_color_rom({m_color_rom.begin(), m_color_rom.end()});
        m_gui->load_palette_rom({m_palette_rom.begin(), m_palette_rom.end()});
        m_gui->load_tile_rom({m_tile_rom.begin(), m_tile_rom.end()});
        m_gui->load_sprite_rom({m_sprite_rom.begin(), m_sprite_rom.end()});

        m_memory.attach_memory_mapper_for_write([&](EmulatorMemory &memory, u16 address, u8 value) -> void {
            address &= address_mask;

            if (address <= address_rom_end) {               // ROM: 0x0000 - 0x3fff
            } else if (address < address_in0_beginning) {   // RAM: 0x4000 - 0x4fff
                memory.direct_write(address, value);
            } else if (address <= address_pacman_memory_end) {
                if (address == address_in0_beginning) {
                    m_memory_mapped_io->in0_write(value);
                } else if (address == address_sound_enable) {
                    m_memory_mapped_io->is_sound_enabled(value > 0);
                } else if (address == address_aux_board) {
                    m_memory_mapped_io->is_aux_board_enabled(value > 0);
                } else if (address == address_flip_screen) {
                    m_memory_mapped_io->flip_screen(1);   // TODO: Fix
                } else if (address == address_lamp1 || address == address_lamp2) {
                } else if (address == address_coin_lockout) {
                } else if (address == address_coin_counter) {
                } else if (address_audio_beginning <= address && address <= address_audio_end) {
                    // TODO
                } else if (address_sprite_coords_beginning <= address && address <= address_sprite_coords_end) {
                    memory.direct_write(address, value);
                } else if (address_watchdog_beginning <= address && address <= address_watchdog_end) {
                }
            }
        });

        m_memory.attach_memory_mapper_for_read([&](const EmulatorMemory &memory, u16 address) -> u8 {
            address &= address_mask;

            if (address <= address_ram_end) {
                return memory.direct_read(address);
            } else if (address <= address_pacman_memory_end) {
                if (address == address_flip_screen) {
                    return m_memory_mapped_io->flip_screen();
                } else if (address == address_lamp1 || address == address_lamp2) {
                    return 0;
                } else if (address == address_coin_lockout) {
                    return 0;
                } else if (address == address_coin_counter) {
                } else if (address_in0_beginning <= address && address <= address_in0_end) {
                    return m_memory_mapped_io->in0_read();
                } else if (address_in1_beginning <= address && address <= address_in1_end) {
                    return m_memory_mapped_io->in1_read();
                } else if (address_dipswitches_beginning <= address && address <= address_dipswitches_end) {
                    return m_memory_mapped_io->dipswitches();
                }

                return 0xff;
            } else {
                return 0;
            }
        });
    }

    std::vector<u8> create_empty_vector(std::size_t size) {
        std::vector<u8> vec(size, 0);
        return vec;
    }

    std::vector<u8> Pacman::create_tile_ram() {
        return create_empty_vector(0x43ff - 0x4000);
    }

    std::vector<u8> Pacman::create_palette_ram() {
        return create_empty_vector(0x47ff - 0x4400);
    }

    std::vector<u8> Pacman::create_ram() {
        return create_empty_vector(0x4fef - 0x4800);
    }

    std::vector<u8> Pacman::create_sprite_ram() {
        return create_empty_vector(0x4fff - 0x4ff0);
    }

    std::vector<u8> Pacman::create_memory_mapped_io() {
        return create_empty_vector(0x50ff - 0x5000);
    }

    std::vector<u8> Pacman::fill_remaining(std::size_t memory_size) {
        return create_empty_vector(UINT16_MAX - memory_size);
    }
}
