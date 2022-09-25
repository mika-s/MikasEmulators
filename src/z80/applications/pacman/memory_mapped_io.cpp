#include "memory_mapped_io.h"
#include "crosscutting/util/byte_util.h"

namespace emu::z80::applications::pacman {

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

    void MemoryMappedIo::set_dipswitches(const Settings &settings) {
        switch (settings.m_coins_per_game) {
            case CoinsPerGame::FreePlay:
                set_bit(m_memory[address_dipswitches], dipswitches_coinage_1);
                set_bit(m_memory[address_dipswitches], dipswitches_coinage_2);
                break;
            case CoinsPerGame::OnePerGame:
                unset_bit(m_memory[address_dipswitches], dipswitches_coinage_1);
                set_bit(m_memory[address_dipswitches], dipswitches_coinage_2);
                break;
            case CoinsPerGame::OnePerTwoGames:
                set_bit(m_memory[address_dipswitches], dipswitches_coinage_1);
                unset_bit(m_memory[address_dipswitches], dipswitches_coinage_2);
                break;
            case CoinsPerGame::TwoCoinsPerGame:
                unset_bit(m_memory[address_dipswitches], dipswitches_coinage_1);
                unset_bit(m_memory[address_dipswitches], dipswitches_coinage_2);
            default:
                break;
        }

        switch (settings.m_number_of_lives) {
            case NumberOfLives::One:
                set_bit(m_memory[address_dipswitches], dipswitches_lives_1);
                set_bit(m_memory[address_dipswitches], dipswitches_lives_2);
                break;
            case NumberOfLives::Two:
                unset_bit(m_memory[address_dipswitches], dipswitches_lives_1);
                set_bit(m_memory[address_dipswitches], dipswitches_lives_2);
                break;
            case NumberOfLives::Three:
                set_bit(m_memory[address_dipswitches], dipswitches_lives_1);
                unset_bit(m_memory[address_dipswitches], dipswitches_lives_2);
                break;
            case NumberOfLives::Five:
                unset_bit(m_memory[address_dipswitches], dipswitches_lives_1);
                unset_bit(m_memory[address_dipswitches], dipswitches_lives_2);
            default:
                break;
        }

        switch (settings.m_bonus_life_at) {
            case BonusLifeAt::_10000:
                set_bit(m_memory[address_dipswitches], dipswitches_bonus_life_1);
                set_bit(m_memory[address_dipswitches], dipswitches_bonus_life_2);
                break;
            case BonusLifeAt::_15000:
                unset_bit(m_memory[address_dipswitches], dipswitches_bonus_life_1);
                set_bit(m_memory[address_dipswitches], dipswitches_bonus_life_2);
                break;
            case BonusLifeAt::_20000:
                set_bit(m_memory[address_dipswitches], dipswitches_bonus_life_1);
                unset_bit(m_memory[address_dipswitches], dipswitches_bonus_life_2);
                break;
            case BonusLifeAt::None:
                unset_bit(m_memory[address_dipswitches], dipswitches_bonus_life_1);
                unset_bit(m_memory[address_dipswitches], dipswitches_bonus_life_2);
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

    void MemoryMappedIo::set_initial_values() {
        m_memory[address_in0] = 0b10011111;
        m_memory[address_in1] = 0xff;
    }

    void MemoryMappedIo::memory_changed(u16 address) {
        [[maybe_unused]] bool is_relevant = from_address <= address && address <= to_address;
    }
}
