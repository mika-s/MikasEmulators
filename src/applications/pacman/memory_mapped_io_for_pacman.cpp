#include "memory_mapped_io_for_pacman.h"
#include "chips/z80/util.h"
#include "crosscutting/util/byte_util.h"

namespace emu::applications::pacman {

    using emu::util::byte::is_bit_set;
    using emu::util::byte::set_bit;
    using emu::util::byte::unset_bit;
    using emu::z80::set_bit_in_memory;
    using emu::z80::unset_bit_in_memory;

    MemoryMappedIoForPacman::MemoryMappedIoForPacman(EmulatorMemory &memory)
            : m_memory(memory) {
    }

    void MemoryMappedIoForPacman::write(u16 address, u8 value) {
        address &= address_mask;

        if (address <= address_rom_end) {
        } else if (address < address_in0_beginning) {
            m_memory.direct_write(address, value);
        } else if (address <= address_pacman_memory_end) {
            if (address == address_in0_beginning) {
                in0_write(value);
            } else if (address == address_sound_enable) {
                is_sound_enabled(value > 0);
            } else if (address == address_aux_board) {
                is_aux_board_enabled(value > 0);
            } else if (address == address_flip_screen) {
                flip_screen(1);   // TODO: Fix
            } else if (address == address_lamp1 || address == address_lamp2) {
            } else if (address == address_coin_lockout) {
            } else if (address == address_coin_counter) {
            } else if (address_audio_beginning <= address && address <= address_audio_end) {
                // TODO
            } else if (address_sprite_coords_beginning <= address && address <= address_sprite_coords_end) {
                m_memory.direct_write(address, value);
            } else if (address_watchdog_beginning <= address && address <= address_watchdog_end) {
            }
        }
    }

    u8 MemoryMappedIoForPacman::read(u16 address) {
        address &= address_mask;

        if (address <= address_ram_end) {
            return m_memory.direct_read(address);
        } else if (address <= address_pacman_memory_end) {
            if (address == address_flip_screen) {
                return flip_screen();
            } else if (address == address_lamp1 || address == address_lamp2) {
                return 0;
            } else if (address == address_coin_lockout) {
                return 0;
            } else if (address == address_coin_counter) {
            } else if (address_in0_beginning <= address && address <= address_in0_end) {
                return in0_read();
            } else if (address_in1_beginning <= address && address <= address_in1_end) {
                return in1_read();
            } else if (address_dipswitches_beginning <= address && address <= address_dipswitches_end) {
                return dipswitches();
            }

            return 0xff;
        } else {
            return 0;
        }
    }

    bool MemoryMappedIoForPacman::is_interrupt_enabled() {
        return is_bit_set(m_in0_write, 0);
    }

    void MemoryMappedIoForPacman::in0_read(unsigned int bit_number, bool is_setting) {
        if (is_setting) {
            set_bit(m_in0_read, bit_number);
        } else {
            unset_bit(m_in0_read, bit_number);
        }
    }

    u8 MemoryMappedIoForPacman::in0_read() const {
        return m_in0_read;
    }

    void MemoryMappedIoForPacman::in1_read(unsigned int bit_number, bool is_setting) {
        if (is_setting) {
            set_bit(m_in1_read, bit_number);
        } else {
            unset_bit(m_in1_read, bit_number);
        }
    }

    u8 MemoryMappedIoForPacman::in1_read() const {
        return m_in1_read;
    }

    void MemoryMappedIoForPacman::in0_write(u8 value) {
        m_in0_write = value;
    }

    u8 MemoryMappedIoForPacman::coin_counter() {
        return 0;
    }

    u8 MemoryMappedIoForPacman::flip_screen() {
        return 0;
    }

    void MemoryMappedIoForPacman::flip_screen([[maybe_unused]] u8 value) {
    }

    void MemoryMappedIoForPacman::dipswitches(const Settings &settings) {
        switch (settings.m_coins_per_game) {
            case CoinsPerGame::FreePlay:
                unset_bit(m_dipswitches, dipswitches_coinage_1);
                unset_bit(m_dipswitches, dipswitches_coinage_2);
                break;
            case CoinsPerGame::_1C1G:
                set_bit(m_dipswitches, dipswitches_coinage_1);
                unset_bit(m_dipswitches, dipswitches_coinage_2);
                break;
            case CoinsPerGame::_1C2G:
                unset_bit(m_dipswitches, dipswitches_coinage_1);
                set_bit(m_dipswitches, dipswitches_coinage_2);
                break;
            case CoinsPerGame::_2C1G:
                set_bit(m_dipswitches, dipswitches_coinage_1);
                set_bit(m_dipswitches, dipswitches_coinage_2);
        }

        switch (settings.m_number_of_lives) {
            case NumberOfLives::One:
                unset_bit(m_dipswitches, dipswitches_lives_1);
                unset_bit(m_dipswitches, dipswitches_lives_2);
                break;
            case NumberOfLives::Two:
                set_bit(m_dipswitches, dipswitches_lives_1);
                unset_bit(m_dipswitches, dipswitches_lives_2);
                break;
            case NumberOfLives::Three:
                unset_bit(m_dipswitches, dipswitches_lives_1);
                set_bit(m_dipswitches, dipswitches_lives_2);
                break;
            case NumberOfLives::Five:
                set_bit(m_dipswitches, dipswitches_lives_1);
                set_bit(m_dipswitches, dipswitches_lives_2);
        }

        switch (settings.m_bonus_life_at) {
            case BonusLifeAt::_10000:
                unset_bit(m_dipswitches, dipswitches_bonus_life_1);
                unset_bit(m_dipswitches, dipswitches_bonus_life_2);
                break;
            case BonusLifeAt::_15000:
                set_bit(m_dipswitches, dipswitches_bonus_life_1);
                unset_bit(m_dipswitches, dipswitches_bonus_life_2);
                break;
            case BonusLifeAt::_20000:
                unset_bit(m_dipswitches, dipswitches_bonus_life_1);
                set_bit(m_dipswitches, dipswitches_bonus_life_2);
                break;
            case BonusLifeAt::None:
                set_bit(m_dipswitches, dipswitches_bonus_life_1);
                set_bit(m_dipswitches, dipswitches_bonus_life_2);
        }

        switch (settings.m_difficulty) {
            case Difficulty::Normal:
                set_bit(m_dipswitches, dipswitches_difficulty);
                break;
            case Difficulty::Hard:
                unset_bit(m_dipswitches, dipswitches_difficulty);
                break;
        }

        switch (settings.m_ghost_names) {
            case GhostNames::Normal:
                set_bit(m_dipswitches, dipswitches_ghost_names);
                break;
            case GhostNames::Alternate:
                unset_bit(m_dipswitches, dipswitches_ghost_names);
                break;
        }
    }

    u8 MemoryMappedIoForPacman::dipswitches() {
        return m_dipswitches;
    }

    void MemoryMappedIoForPacman::board_test(const Settings &settings) {
        switch (settings.m_board_test) {
            case BoardTest::Off:
                set_bit(m_in1_read, board_test_bit);
                break;
            case BoardTest::On:
                unset_bit(m_in1_read, board_test_bit);
                break;
        }
    }

    bool MemoryMappedIoForPacman::is_sound_enabled() {
        return m_is_sound_enabled;
    }

    void MemoryMappedIoForPacman::is_sound_enabled(bool new_value) {
        m_is_sound_enabled = new_value;
    }

    bool MemoryMappedIoForPacman::is_aux_board_enabled() {
        return m_is_aux_board_enabled;
    }

    void MemoryMappedIoForPacman::is_aux_board_enabled(bool new_value) {
        m_is_aux_board_enabled = new_value;
    }
}
