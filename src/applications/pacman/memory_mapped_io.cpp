#include "memory_mapped_io.h"
#include "crosscutting/util/byte_util.h"

namespace emu::applications::pacman {

    using emu::util::byte::is_bit_set;
    using emu::util::byte::set_bit;
    using emu::util::byte::unset_bit;

    MemoryMappedIo::MemoryMappedIo(EmulatorMemory &memory)
            : m_memory(memory) {
        set_initial_values();
    }

    bool MemoryMappedIo::is_interrupt_enabled() {
        return read_in0(0);
    }

    bool MemoryMappedIo::read_in0(unsigned int bit_number) {
        return is_bit_set(m_memory[address_in0], bit_number);
    }

    u8 MemoryMappedIo::read_in0() const {
        return m_memory[address_in0];
    }

    u8 MemoryMappedIo::read_in1() const {
        return m_memory[address_in1];
    }

    void MemoryMappedIo::write_in0(unsigned int bit_number, bool is_setting) {
        if (is_setting) {
            set_bit(m_memory[address_in0], bit_number);
        } else {
            unset_bit(m_memory[address_in0], bit_number);
        }
    }

    void MemoryMappedIo::write_in1(unsigned int bit_number, bool is_setting) {
        if (is_setting) {
            set_bit(m_memory[address_in1], bit_number);
        } else {
            unset_bit(m_memory[address_in1], bit_number);
        }
    }

    u8 MemoryMappedIo::read_coin_counter() {
        return m_memory[address_coin_counter];
    }

    void MemoryMappedIo::set_dipswitches(const Settings &settings) {
        switch (settings.m_coins_per_game) {
            case CoinsPerGame::FreePlay:
                unset_bit(m_memory[address_dipswitches], dipswitches_coinage_1);
                unset_bit(m_memory[address_dipswitches], dipswitches_coinage_2);
                break;
            case CoinsPerGame::_1C1G:
                set_bit(m_memory[address_dipswitches], dipswitches_coinage_1);
                unset_bit(m_memory[address_dipswitches], dipswitches_coinage_2);
                break;
            case CoinsPerGame::_1C2G:
                unset_bit(m_memory[address_dipswitches], dipswitches_coinage_1);
                set_bit(m_memory[address_dipswitches], dipswitches_coinage_2);
                break;
            case CoinsPerGame::_2C1G:
                set_bit(m_memory[address_dipswitches], dipswitches_coinage_1);
                set_bit(m_memory[address_dipswitches], dipswitches_coinage_2);
            default:
                break;
        }

        switch (settings.m_number_of_lives) {
            case NumberOfLives::One:
                unset_bit(m_memory[address_dipswitches], dipswitches_lives_1);
                unset_bit(m_memory[address_dipswitches], dipswitches_lives_2);
                break;
            case NumberOfLives::Two:
                set_bit(m_memory[address_dipswitches], dipswitches_lives_1);
                unset_bit(m_memory[address_dipswitches], dipswitches_lives_2);
                break;
            case NumberOfLives::Three:
                unset_bit(m_memory[address_dipswitches], dipswitches_lives_1);
                set_bit(m_memory[address_dipswitches], dipswitches_lives_2);
                break;
            case NumberOfLives::Five:
                set_bit(m_memory[address_dipswitches], dipswitches_lives_1);
                set_bit(m_memory[address_dipswitches], dipswitches_lives_2);
            default:
                break;
        }

        switch (settings.m_bonus_life_at) {
            case BonusLifeAt::_10000:
                unset_bit(m_memory[address_dipswitches], dipswitches_bonus_life_1);
                unset_bit(m_memory[address_dipswitches], dipswitches_bonus_life_2);
                break;
            case BonusLifeAt::_15000:
                set_bit(m_memory[address_dipswitches], dipswitches_bonus_life_1);
                unset_bit(m_memory[address_dipswitches], dipswitches_bonus_life_2);
                break;
            case BonusLifeAt::_20000:
                unset_bit(m_memory[address_dipswitches], dipswitches_bonus_life_1);
                set_bit(m_memory[address_dipswitches], dipswitches_bonus_life_2);
                break;
            case BonusLifeAt::None:
                set_bit(m_memory[address_dipswitches], dipswitches_bonus_life_1);
                set_bit(m_memory[address_dipswitches], dipswitches_bonus_life_2);
            default:
                break;
        }

        switch (settings.m_difficulty) {
            case Difficulty::Normal:
                set_bit(m_memory[address_dipswitches], dipswitches_difficulty);
                break;
            case Difficulty::Hard:
                unset_bit(m_memory[address_dipswitches], dipswitches_difficulty);
                break;
            default:
                break;
        }

        switch (settings.m_ghost_names) {
            case GhostNames::Normal:
                set_bit(m_memory[address_dipswitches], dipswitches_ghost_names);
                break;
            case GhostNames::Alternate:
                unset_bit(m_memory[address_dipswitches], dipswitches_ghost_names);
                break;
            default:
                break;
        }
    }

    u8 MemoryMappedIo::read_dipswitches() {
        return m_memory[address_dipswitches];
    }

    void MemoryMappedIo::set_board_test(const Settings &settings) {
        switch (settings.m_board_test) {
            case BoardTest::Off:
                set_bit(m_memory[address_in1], board_test);
                set_bit(m_initial_value_in1, board_test);
                break;
            case BoardTest::On:
                unset_bit(m_memory[address_in1], board_test);
                unset_bit(m_initial_value_in1, board_test);
                break;
            default:
                break;
        }
    }

    void MemoryMappedIo::set_initial_values() {
        m_memory[address_in0] = m_initial_value_in0;
        m_memory[address_in1] = m_initial_value_in1;
    }
}
