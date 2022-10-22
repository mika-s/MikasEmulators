#include "memory_mapped_io.h"
#include "chips/z80/util.h"
#include "crosscutting/util/byte_util.h"

namespace emu::applications::pacman {

    using emu::util::byte::is_bit_set;
    using emu::util::byte::set_bit;
    using emu::util::byte::unset_bit;
    using emu::z80::set_bit_in_memory;
    using emu::z80::unset_bit_in_memory;

    bool MemoryMappedIo::is_interrupt_enabled() {
        return is_bit_set(m_in0_write, 0);
    }

    void MemoryMappedIo::in0_read(unsigned int bit_number, bool is_setting) {
        if (is_setting) {
            set_bit(m_in0_read, bit_number);
        } else {
            unset_bit(m_in0_read, bit_number);
        }
    }

    u8 MemoryMappedIo::in0_read() const {
        return m_in0_read;
    }

    void MemoryMappedIo::in1_read(unsigned int bit_number, bool is_setting) {
        if (is_setting) {
            set_bit(m_in1_read, bit_number);
        } else {
            unset_bit(m_in1_read, bit_number);
        }
    }

    u8 MemoryMappedIo::in1_read() const {
        return m_in1_read;
    }

    u8 MemoryMappedIo::in0_write() const {
        return m_in0_write;
    }

    u8 MemoryMappedIo::in1_write() const {
        return m_in1_write;
    }

    void MemoryMappedIo::in0_write(u8 value) {
        m_in0_write = value;
    }

    void MemoryMappedIo::in0_write(unsigned int bit_number, bool is_setting) {
        if (is_setting) {
            set_bit(m_in0_write, bit_number);
        } else {
            unset_bit(m_in0_write, bit_number);
        }
    }

    void MemoryMappedIo::in1_write(u8 value) {
        m_in1_write = value;
    }

    void MemoryMappedIo::in1_write(unsigned int bit_number, bool is_setting) {
        if (is_setting) {
            set_bit(m_in1_write, bit_number);
        } else {
            unset_bit(m_in1_write, bit_number);
        }
    }

    u8 MemoryMappedIo::coin_counter() {
        return 0;
    }

    u8 MemoryMappedIo::flip_screen() {
        return 0;
    }

    void MemoryMappedIo::flip_screen([[maybe_unused]] u8 value) {
    }

    void MemoryMappedIo::dipswitches(const Settings &settings) {
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

    u8 MemoryMappedIo::dipswitches() {
        return m_dipswitches;
    }

    void MemoryMappedIo::board_test(const Settings &settings) {
        switch (settings.m_board_test) {
            case BoardTest::Off:
                set_bit(m_dipswitches, board_test_bit);
                break;
            case BoardTest::On:
                unset_bit(m_dipswitches, board_test_bit);
                break;
        }
    }

    bool MemoryMappedIo::is_sound_enabled() {
        return m_is_sound_enabled;
    }

    void MemoryMappedIo::is_sound_enabled(bool new_value) {
        m_is_sound_enabled = new_value;
    }

    bool MemoryMappedIo::is_aux_board_enabled() {
        return m_is_aux_board_enabled;
    }

    void MemoryMappedIo::is_aux_board_enabled(bool new_value) {
        m_is_aux_board_enabled = new_value;
    }
}
