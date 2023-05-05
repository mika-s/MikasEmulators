#include "memory_mapped_io_for_game_boy.h"
#include "chips/z80/util.h"
#include "crosscutting/memory/emulator_memory.h"
#include "game_boy/settings.h"
#include "namco_wsg3/voice.h"

namespace emu::applications::game_boy {

using emu::util::byte::is_bit_set;
using emu::util::byte::set_bit;
using emu::util::byte::unset_bit;
using emu::z80::set_bit_in_memory;
using emu::z80::unset_bit_in_memory;

MemoryMappedIoForGameBoy::MemoryMappedIoForGameBoy(EmulatorMemory<u16, u8>& memory, Settings settings)
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
void MemoryMappedIoForGameBoy::write(u16 address, u8 value)
{
    address &= s_address_mask;

    if (address <= s_address_rom_end) {
    } else if (address < s_address_in0_beginning) {
        m_memory.direct_write(address, value);
    } else if (address <= s_address_pacman_memory_end) {
        if (address == s_address_in0_beginning) {
            in0_write(value);
        } else if (address == s_address_sound_enable) {
            is_sound_enabled(value);
        } else if (address == s_address_aux_board) {
            is_aux_board_enabled(value);
        } else if (address == s_address_flip_screen) {
            flip_screen(value);
        } else if (address == s_address_lamp1 || address == s_address_lamp2) {
        } else if (address == s_address_coin_lockout) {
        } else if (address == s_address_coin_counter) {
        } else if (s_address_audio_beginning <= address && address <= s_address_audio_end) {
            if (s_address_voice1_sound_beginning <= address && address <= s_address_voice1_sound_end) {
                voice1_accumulator(value, address);
            } else if (address == s_address_voice1_waveform) {
                voice1_waveform(value);
            } else if (s_address_voice2_sound_beginning <= address && address <= s_address_voice2_sound_end) {
                voice2_accumulator(value, address);
            } else if (address == s_address_voice2_waveform) {
                voice2_waveform(value);
            } else if (s_address_voice3_sound_beginning <= address && address <= s_address_voice3_sound_end) {
                voice3_accumulator(value, address);
            } else if (address == s_address_voice3_waveform) {
                voice3_waveform(value);
            } else if (s_address_voice1_frequency_beginning <= address && address <= s_address_voice1_frequency_end) {
                voice1_frequency(value, address);
            } else if (address == s_address_voice1_volume) {
                voice1_volume(value);
            } else if (s_address_voice2_frequency_beginning <= address && address <= s_address_voice2_frequency_end) {
                voice2_frequency(value, address);
            } else if (address == s_address_voice2_volume) {
                voice2_volume(value);
            } else if (s_address_voice3_frequency_beginning <= address && address <= s_address_voice3_frequency_end) {
                voice3_frequency(value, address);
            } else if (address == s_address_voice3_volume) {
                voice3_volume(value);
            }
        } else if (s_address_sprite_coords_beginning <= address && address <= s_address_sprite_coords_end) {
            m_memory.direct_write(address, value);
        } else if (s_address_watchdog_beginning <= address && address <= s_address_watchdog_end) {
        }
    }
}

/**
 * Called when the CPU reads from memory
 *
 * @param address is the address in memory to read from
 * @return the value in memory at the given address
 */
u8 MemoryMappedIoForGameBoy::read(u16 address)
{
    address &= s_address_mask;

    if (address <= s_address_ram_end) {
        return m_memory.direct_read(address);
    } else if (address <= s_address_pacman_memory_end) {
        if (address == s_address_lamp1 || address == s_address_lamp2) {
            return 0;
        } else if (address == s_address_coin_lockout) {
            return 0;
        } else if (address == s_address_coin_counter) {
        } else if (s_address_in0_beginning <= address && address <= s_address_in0_end) {
            return in0_read();
        } else if (s_address_in1_beginning <= address && address <= s_address_in1_end) {
            return in1_read();
        } else if (s_address_dipswitches_beginning <= address && address <= s_address_dipswitches_end) {
            return dipswitches();
        }

        return 0xff;
    } else {
        return 0;
    }
}

bool MemoryMappedIoForGameBoy::is_interrupt_enabled()
{
    return is_bit_set(m_in0_write, 0);
}

void MemoryMappedIoForGameBoy::in0_read(unsigned int bit_number, bool is_setting)
{
    if (is_setting) {
        set_bit(m_in0_read, bit_number);
    } else {
        unset_bit(m_in0_read, bit_number);
    }
}

u8 MemoryMappedIoForGameBoy::in0_read() const
{
    return m_in0_read;
}

void MemoryMappedIoForGameBoy::in1_read(unsigned int bit_number, bool is_setting)
{
    if (is_setting) {
        set_bit(m_in1_read, bit_number);
    } else {
        unset_bit(m_in1_read, bit_number);
    }
}

u8 MemoryMappedIoForGameBoy::in1_read() const
{
    return m_in1_read;
}

void MemoryMappedIoForGameBoy::in0_write(u8 value)
{
    m_in0_write = value;
}

u8 MemoryMappedIoForGameBoy::coin_counter()
{
    return 0;
}

void MemoryMappedIoForGameBoy::flip_screen(u8 value)
{
    m_is_screen_flipped = value > 0;
}

void MemoryMappedIoForGameBoy::dipswitches(Settings const& settings)
{
    switch (settings.m_coins_per_game) {
    case CoinsPerGame::FreePlay:
        unset_bit(m_dipswitches, s_dipswitches_coinage_1);
        unset_bit(m_dipswitches, s_dipswitches_coinage_2);
        break;
    case CoinsPerGame::_1C1G:
        set_bit(m_dipswitches, s_dipswitches_coinage_1);
        unset_bit(m_dipswitches, s_dipswitches_coinage_2);
        break;
    case CoinsPerGame::_1C2G:
        unset_bit(m_dipswitches, s_dipswitches_coinage_1);
        set_bit(m_dipswitches, s_dipswitches_coinage_2);
        break;
    case CoinsPerGame::_2C1G:
        set_bit(m_dipswitches, s_dipswitches_coinage_1);
        set_bit(m_dipswitches, s_dipswitches_coinage_2);
    }

    switch (settings.m_number_of_lives) {
    case NumberOfLives::One:
        unset_bit(m_dipswitches, s_dipswitches_lives_1);
        unset_bit(m_dipswitches, s_dipswitches_lives_2);
        break;
    case NumberOfLives::Two:
        set_bit(m_dipswitches, s_dipswitches_lives_1);
        unset_bit(m_dipswitches, s_dipswitches_lives_2);
        break;
    case NumberOfLives::Three:
        unset_bit(m_dipswitches, s_dipswitches_lives_1);
        set_bit(m_dipswitches, s_dipswitches_lives_2);
        break;
    case NumberOfLives::Five:
        set_bit(m_dipswitches, s_dipswitches_lives_1);
        set_bit(m_dipswitches, s_dipswitches_lives_2);
    }

    switch (settings.m_bonus_life_at) {
    case BonusLifeAt::_10000:
        unset_bit(m_dipswitches, s_dipswitches_bonus_life_1);
        unset_bit(m_dipswitches, s_dipswitches_bonus_life_2);
        break;
    case BonusLifeAt::_15000:
        set_bit(m_dipswitches, s_dipswitches_bonus_life_1);
        unset_bit(m_dipswitches, s_dipswitches_bonus_life_2);
        break;
    case BonusLifeAt::_20000:
        unset_bit(m_dipswitches, s_dipswitches_bonus_life_1);
        set_bit(m_dipswitches, s_dipswitches_bonus_life_2);
        break;
    case BonusLifeAt::None:
        set_bit(m_dipswitches, s_dipswitches_bonus_life_1);
        set_bit(m_dipswitches, s_dipswitches_bonus_life_2);
    }

    switch (settings.m_difficulty) {
    case Difficulty::Normal:
        set_bit(m_dipswitches, s_dipswitches_difficulty);
        break;
    case Difficulty::Hard:
        unset_bit(m_dipswitches, s_dipswitches_difficulty);
        break;
    }

    switch (settings.m_ghost_names) {
    case GhostNames::Normal:
        set_bit(m_dipswitches, s_dipswitches_ghost_names);
        break;
    case GhostNames::Alternate:
        unset_bit(m_dipswitches, s_dipswitches_ghost_names);
        break;
    }
}

u8 MemoryMappedIoForGameBoy::dipswitches()
{
    return m_dipswitches;
}

void MemoryMappedIoForGameBoy::board_test(Settings const& settings)
{
    switch (settings.m_board_test) {
    case BoardTest::Off:
        set_bit(m_in1_read, s_board_test_bit);
        break;
    case BoardTest::On:
        unset_bit(m_in1_read, s_board_test_bit);
        break;
    }
}

void MemoryMappedIoForGameBoy::cabinet_mode(Settings const& settings)
{
    switch (settings.m_cabinet_mode) {
    case CabinetMode::Table:
        unset_bit(m_in1_read, s_cabinet_mode_bit);
        break;
    case CabinetMode::Upright:
        set_bit(m_in1_read, s_cabinet_mode_bit);
        break;
    }
}

bool MemoryMappedIoForGameBoy::is_sound_enabled()
{
    return m_is_sound_enabled;
}

void MemoryMappedIoForGameBoy::is_sound_enabled(u8 value)
{
    m_is_sound_enabled = is_bit_set(value, s_sound_enabled_bit);
}

bool MemoryMappedIoForGameBoy::is_aux_board_enabled()
{
    return m_is_aux_board_enabled;
}

void MemoryMappedIoForGameBoy::is_aux_board_enabled(u8 value)
{
    m_is_aux_board_enabled = value > 0;
}

bool MemoryMappedIoForGameBoy::is_screen_flipped()
{
    return m_is_screen_flipped;
}

std::vector<Voice>& MemoryMappedIoForGameBoy::voices()
{
    return m_voices;
}

void MemoryMappedIoForGameBoy::voice1_accumulator(u8 value, u16 address)
{
    const u8 sample = address - s_address_voice1_sound_beginning;
    m_voices[0].accumulator(m_voices[0].accumulator() & ~(0x0f << (sample * 4)));
    m_voices[0].accumulator(m_voices[0].accumulator() | (low_nibble(value) << (sample * 4)));
}

void MemoryMappedIoForGameBoy::voice1_waveform(u8 value)
{
    m_voices[0].waveform_number(value & 0b111);
}

void MemoryMappedIoForGameBoy::voice1_frequency(u8 frequency, u16 address)
{
    const u8 sample = address - s_address_voice1_frequency_beginning;
    m_voices[0].frequency(m_voices[0].frequency() & ~(0x0f << (sample * 4)));
    m_voices[0].frequency(m_voices[0].frequency() | (low_nibble(frequency) << (sample * 4)));
}

void MemoryMappedIoForGameBoy::voice1_volume(u8 volume)
{
    m_voices[0].volume(low_nibble(volume));
}

void MemoryMappedIoForGameBoy::voice2_accumulator(u8 value, u16 address)
{
    const u8 sample = address - s_address_voice2_sound_beginning + 1;
    m_voices[1].accumulator(m_voices[1].accumulator() & ~(0x0f << (sample * 4)));
    m_voices[1].accumulator(m_voices[1].accumulator() | (low_nibble(value) << (sample * 4)));
}

void MemoryMappedIoForGameBoy::voice2_waveform(u8 value)
{
    m_voices[1].waveform_number(value & 0b111);
}

void MemoryMappedIoForGameBoy::voice2_frequency(u8 frequency, u16 address)
{
    const u8 sample = address - s_address_voice2_frequency_beginning + 1;
    m_voices[1].frequency(m_voices[1].frequency() & ~(0x0f << (sample * 4)));
    m_voices[1].frequency(m_voices[1].frequency() | (low_nibble(frequency) << (sample * 4)));
}

void MemoryMappedIoForGameBoy::voice2_volume(u8 volume)
{
    m_voices[1].volume(low_nibble(volume));
}

void MemoryMappedIoForGameBoy::voice3_accumulator(u8 value, u16 address)
{
    const u8 sample = address - s_address_voice3_sound_beginning + 1;
    m_voices[2].accumulator(m_voices[2].accumulator() & ~(0x0f << (sample * 4)));
    m_voices[2].accumulator(m_voices[2].accumulator() | (low_nibble(value) << (sample * 4)));
}

void MemoryMappedIoForGameBoy::voice3_waveform(u8 value)
{
    m_voices[2].waveform_number(value & 0b111);
}

void MemoryMappedIoForGameBoy::voice3_frequency(u8 frequency, u16 address)
{
    const u8 sample = address - s_address_voice3_frequency_beginning + 1;
    m_voices[2].frequency(m_voices[2].frequency() & ~(0x0f << (sample * 4)));
    m_voices[2].frequency(m_voices[2].frequency() | (low_nibble(frequency) << (sample * 4)));
}

void MemoryMappedIoForGameBoy::voice3_volume(u8 volume)
{
    m_voices[2].volume(low_nibble(volume));
}
}
