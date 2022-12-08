#include "memory_mapped_io_for_pacman.h"
#include "chips/z80/util.h"
#include "crosscutting/memory/emulator_memory.h"
#include "namco_wsg3/voice.h"
#include "pacman/settings.h"
#include <memory>

namespace emu::applications::pacman {

using emu::util::byte::is_bit_set;
using emu::util::byte::set_bit;
using emu::util::byte::unset_bit;
using emu::z80::set_bit_in_memory;
using emu::z80::unset_bit_in_memory;

MemoryMappedIoForPacman::MemoryMappedIoForPacman(EmulatorMemory<u16, u8>& memory, Settings settings)
    : m_memory(memory)
    , m_is_sound_enabled(false)
    , m_is_aux_board_enabled(false)
    , m_is_screen_flipped(false)
    , m_voices({ Voice(), Voice(), Voice() })
{

    dipswitches(settings);
    board_test(settings);
    cabinet_mode(settings);
}

/**
 * Called when the CPU writes to memory
 *
 * @param address is the address in memory to write to
 * @param value is the value that should be written to memory
 */
void MemoryMappedIoForPacman::write(u16 address, u8 value)
{
    address &= address_mask;

    if (address <= address_rom_end) {
    } else if (address < address_in0_beginning) {
        m_memory.direct_write(address, value);
    } else if (address <= address_pacman_memory_end) {
        if (address == address_in0_beginning) {
            in0_write(value);
        } else if (address == address_sound_enable) {
            is_sound_enabled(value);
        } else if (address == address_aux_board) {
            is_aux_board_enabled(value);
        } else if (address == address_flip_screen) {
            flip_screen(value);
        } else if (address == address_lamp1 || address == address_lamp2) {
        } else if (address == address_coin_lockout) {
        } else if (address == address_coin_counter) {
        } else if (address_audio_beginning <= address && address <= address_audio_end) {
            if (address_voice1_sound_beginning <= address && address <= address_voice1_sound_end) {
                voice1_accumulator(value, address);
            } else if (address == address_voice1_waveform) {
                voice1_waveform(value);
            } else if (address_voice2_sound_beginning <= address && address <= address_voice2_sound_end) {
                voice2_accumulator(value, address);
            } else if (address == address_voice2_waveform) {
                voice2_waveform(value);
            } else if (address_voice3_sound_beginning <= address && address <= address_voice3_sound_end) {
                voice3_accumulator(value, address);
            } else if (address == address_voice3_waveform) {
                voice3_waveform(value);
            } else if (address_voice1_frequency_beginning <= address && address <= address_voice1_frequency_end) {
                voice1_frequency(value, address);
            } else if (address == address_voice1_volume) {
                voice1_volume(value);
            } else if (address_voice2_frequency_beginning <= address && address <= address_voice2_frequency_end) {
                voice2_frequency(value, address);
            } else if (address == address_voice2_volume) {
                voice2_volume(value);
            } else if (address_voice3_frequency_beginning <= address && address <= address_voice3_frequency_end) {
                voice3_frequency(value, address);
            } else if (address == address_voice3_volume) {
                voice3_volume(value);
            }
        } else if (address_sprite_coords_beginning <= address && address <= address_sprite_coords_end) {
            m_memory.direct_write(address, value);
        } else if (address_watchdog_beginning <= address && address <= address_watchdog_end) {
        }
    }
}

/**
 * Called when the CPU reads from memory
 *
 * @param address is the address in memory to read from
 * @return the value in memory at the given address
 */
u8 MemoryMappedIoForPacman::read(u16 address)
{
    address &= address_mask;

    if (address <= address_ram_end) {
        return m_memory.direct_read(address);
    } else if (address <= address_pacman_memory_end) {
        if (address == address_lamp1 || address == address_lamp2) {
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

bool MemoryMappedIoForPacman::is_interrupt_enabled()
{
    return is_bit_set(m_in0_write, 0);
}

void MemoryMappedIoForPacman::in0_read(unsigned int bit_number, bool is_setting)
{
    if (is_setting) {
        set_bit(m_in0_read, bit_number);
    } else {
        unset_bit(m_in0_read, bit_number);
    }
}

u8 MemoryMappedIoForPacman::in0_read() const
{
    return m_in0_read;
}

void MemoryMappedIoForPacman::in1_read(unsigned int bit_number, bool is_setting)
{
    if (is_setting) {
        set_bit(m_in1_read, bit_number);
    } else {
        unset_bit(m_in1_read, bit_number);
    }
}

u8 MemoryMappedIoForPacman::in1_read() const
{
    return m_in1_read;
}

void MemoryMappedIoForPacman::in0_write(u8 value)
{
    m_in0_write = value;
}

u8 MemoryMappedIoForPacman::coin_counter()
{
    return 0;
}

void MemoryMappedIoForPacman::flip_screen(u8 value)
{
    m_is_screen_flipped = value > 0;
}

void MemoryMappedIoForPacman::dipswitches(Settings const& settings)
{
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

u8 MemoryMappedIoForPacman::dipswitches()
{
    return m_dipswitches;
}

void MemoryMappedIoForPacman::board_test(Settings const& settings)
{
    switch (settings.m_board_test) {
    case BoardTest::Off:
        set_bit(m_in1_read, board_test_bit);
        break;
    case BoardTest::On:
        unset_bit(m_in1_read, board_test_bit);
        break;
    }
}

void MemoryMappedIoForPacman::cabinet_mode(Settings const& settings)
{
    switch (settings.m_cabinet_mode) {
    case CabinetMode::Table:
        unset_bit(m_in1_read, cabinet_mode_bit);
        break;
    case CabinetMode::Upright:
        set_bit(m_in1_read, cabinet_mode_bit);
        break;
    }
}

bool MemoryMappedIoForPacman::is_sound_enabled()
{
    return m_is_sound_enabled;
}

void MemoryMappedIoForPacman::is_sound_enabled(u8 value)
{
    m_is_sound_enabled = is_bit_set(value, sound_enabled_bit);
}

bool MemoryMappedIoForPacman::is_aux_board_enabled()
{
    return m_is_aux_board_enabled;
}

void MemoryMappedIoForPacman::is_aux_board_enabled(u8 value)
{
    m_is_aux_board_enabled = value > 0;
}

bool MemoryMappedIoForPacman::is_screen_flipped()
{
    return m_is_screen_flipped;
}

std::vector<Voice>& MemoryMappedIoForPacman::voices()
{
    return m_voices;
}

void MemoryMappedIoForPacman::voice1_accumulator(u8 value, u16 address)
{
    const u8 sample = address - address_voice1_sound_beginning;
    m_voices[0].accumulator(m_voices[0].accumulator() & ~(0x0f << (sample * 4)));
    m_voices[0].accumulator(m_voices[0].accumulator() | (low_nibble(value) << (sample * 4)));
}

void MemoryMappedIoForPacman::voice1_waveform(u8 value)
{
    m_voices[0].waveform_number(value & 0b111);
}

void MemoryMappedIoForPacman::voice1_frequency(u8 frequency, u16 address)
{
    const u8 sample = address - address_voice1_frequency_beginning;
    m_voices[0].frequency(m_voices[0].frequency() & ~(0x0f << (sample * 4)));
    m_voices[0].frequency(m_voices[0].frequency() | (low_nibble(frequency) << (sample * 4)));
}

void MemoryMappedIoForPacman::voice1_volume(u8 volume)
{
    m_voices[0].volume(low_nibble(volume));
}

void MemoryMappedIoForPacman::voice2_accumulator(u8 value, u16 address)
{
    const u8 sample = address - address_voice2_sound_beginning + 1;
    m_voices[1].accumulator(m_voices[1].accumulator() & ~(0x0f << (sample * 4)));
    m_voices[1].accumulator(m_voices[1].accumulator() | (low_nibble(value) << (sample * 4)));
}

void MemoryMappedIoForPacman::voice2_waveform(u8 value)
{
    m_voices[1].waveform_number(value & 0b111);
}

void MemoryMappedIoForPacman::voice2_frequency(u8 frequency, u16 address)
{
    const u8 sample = address - address_voice2_frequency_beginning + 1;
    m_voices[1].frequency(m_voices[1].frequency() & ~(0x0f << (sample * 4)));
    m_voices[1].frequency(m_voices[1].frequency() | (low_nibble(frequency) << (sample * 4)));
}

void MemoryMappedIoForPacman::voice2_volume(u8 volume)
{
    m_voices[1].volume(low_nibble(volume));
}

void MemoryMappedIoForPacman::voice3_accumulator(u8 value, u16 address)
{
    const u8 sample = address - address_voice3_sound_beginning + 1;
    m_voices[2].accumulator(m_voices[2].accumulator() & ~(0x0f << (sample * 4)));
    m_voices[2].accumulator(m_voices[2].accumulator() | (low_nibble(value) << (sample * 4)));
}

void MemoryMappedIoForPacman::voice3_waveform(u8 value)
{
    m_voices[2].waveform_number(value & 0b111);
}

void MemoryMappedIoForPacman::voice3_frequency(u8 frequency, u16 address)
{
    const u8 sample = address - address_voice3_frequency_beginning + 1;
    m_voices[2].frequency(m_voices[2].frequency() & ~(0x0f << (sample * 4)));
    m_voices[2].frequency(m_voices[2].frequency() | (low_nibble(frequency) << (sample * 4)));
}

void MemoryMappedIoForPacman::voice3_volume(u8 volume)
{
    m_voices[2].volume(low_nibble(volume));
}
}
