#include "pacman.h"
#include "z80/cpu.h"
#include "crosscutting/util/byte_util.h"
#include "crosscutting/util/file_util.h"

namespace emu::z80::applications::pacman {

    using emu::z80::RunStatus;
    using emu::util::file::read_file_into_vector;

    Pacman::Pacman(
            const Settings &settings,
            std::shared_ptr<Gui> gui,
            std::shared_ptr<Input> input
    )
            : m_settings(settings),
              m_gui(std::move(gui)),
              m_input(std::move(input)) {
        load_file();
    }

    std::unique_ptr<Session> Pacman::new_session() {
        return std::make_unique<PacmanSession>(
                m_settings,
                m_gui,
                m_input,
                m_memory,
                m_tile_rom,
                m_sprite_rom
        );
    }

    void Pacman::load_file() {
        const std::string directory = "roms/z80/pacman/";
        m_memory.add(read_file_into_vector(directory + "pacman.6e")); // $0000-$0fff: pacman.6e, code
        m_memory.add(read_file_into_vector(directory + "pacman.6f")); // $1000-$1fff: pacman.6f, code
        m_memory.add(read_file_into_vector(directory + "pacman.6h")); // $2000-$2fff: pacman.6h, code
        m_memory.add(read_file_into_vector(directory + "pacman.6j")); // $3000-$3fff: pacman.6j, code
        m_memory.add(create_tile_ram());                                    // $4000-$43ff: tile RAM
        m_memory.add(create_palette_ram());                                 // $4400-$47ff: palette RAM
        m_memory.add(create_ram());                                         // $4800-$4fef: RAM
        m_memory.add(create_sprite_ram());                                  // $4ff0-$4fff: sprite RAM
        m_memory.add(create_memory_mapped_io());                            // $5000-$50ff: memory-mapped IO

        m_tile_rom.add(read_file_into_vector(directory + "pacman.5e"));   // $0000-$0fff: pacman.5e, tiles
        m_sprite_rom.add(read_file_into_vector(directory + "pacman.5f")); // $0000-$0fff: pacman.5f, sprites
    }

    std::vector<u8> create_empty_vector(int size) {
        std::vector<u8> vec;

        vec.reserve(size);
        for (int i = 0; i <= size; ++i) {
            vec.push_back(0);
        }

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
}
