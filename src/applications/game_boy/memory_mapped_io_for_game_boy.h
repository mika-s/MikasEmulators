#pragma once

#include "chips/namco_wsg3/voice.h"
#include "crosscutting/memory/memory_mapped_io.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include <cstddef>
#include <vector>

namespace emu::applications::game_boy {
class Settings;
}
namespace emu::memory {
template<class A, class D>
class EmulatorMemory;
}

namespace emu::applications::game_boy {

using emu::memory::EmulatorMemory;
using emu::memory::MemoryMappedIo;
using emu::util::byte::low_nibble;
using emu::wsg3::Voice;

class MemoryMappedIoForGameBoy : public MemoryMappedIo<u16, u8> {
public:
    explicit MemoryMappedIoForGameBoy(EmulatorMemory<u16, u8>& memory, Settings settings);

    void write(u16 address, u8 value) override;

    u8 read(u16 address) override;

    bool is_interrupt_enabled();

    void in0_read(unsigned int bit_number, bool is_setting);

    [[nodiscard]] u8 in0_read() const;

    void in1_read(unsigned int bit_number, bool is_setting);

    [[nodiscard]] u8 in1_read() const;

    u8 coin_counter();

    u8 dipswitches();

    bool is_sound_enabled();

    bool is_aux_board_enabled();

    bool is_screen_flipped();

    std::vector<Voice>& voices();

private:
    static constexpr unsigned int s_sound_enabled_bit = 0;

    static constexpr unsigned int s_board_test_bit = 4;
    static constexpr unsigned int s_cabinet_mode_bit = 7;
    static constexpr unsigned int s_dipswitches_coinage_1 = 0;
    static constexpr unsigned int s_dipswitches_coinage_2 = 1;
    static constexpr unsigned int s_dipswitches_lives_1 = 2;
    static constexpr unsigned int s_dipswitches_lives_2 = 3;
    static constexpr unsigned int s_dipswitches_bonus_life_1 = 4;
    static constexpr unsigned int s_dipswitches_bonus_life_2 = 5;
    static constexpr unsigned int s_dipswitches_difficulty = 6;
    static constexpr unsigned int s_dipswitches_ghost_names = 7;
    static constexpr u8 s_initial_value_in0_read = 0b10011111;
    static constexpr u8 s_initial_value_in1_read = 0b11111111;

    static constexpr std::size_t s_address_mask = 0x7fff;
    static constexpr u16 s_address_rom_end = 0x3fff;
    static constexpr u16 s_address_ram_end = 0x4fff;
    static constexpr u16 s_address_in0_beginning = 0x5000;
    static constexpr u16 s_address_in0_end = 0x503f;
    static constexpr u16 s_address_sound_enable = 0x5001;
    static constexpr u16 s_address_aux_board = 0x5002;
    static constexpr u16 s_address_flip_screen = 0x5003;
    static constexpr u16 s_address_lamp1 = 0x5004;
    static constexpr u16 s_address_lamp2 = 0x5005;
    static constexpr u16 s_address_coin_lockout = 0x5006;
    static constexpr u16 s_address_coin_counter = 0x5007;
    static constexpr u16 s_address_in1_beginning = 0x5040;
    static constexpr u16 s_address_in1_end = 0x507f;
    static constexpr u16 s_address_dipswitches_beginning = 0x5080;
    static constexpr u16 s_address_dipswitches_end = 0x50bf;
    static constexpr u16 s_address_audio_beginning = 0x5040;
    static constexpr u16 s_address_audio_end = 0x505f;
    static constexpr u16 s_address_voice1_sound_beginning = 0x5040;
    static constexpr u16 s_address_voice1_sound_end = 0x5044;
    static constexpr u16 s_address_voice1_waveform = 0x5045;
    static constexpr u16 s_address_voice2_sound_beginning = 0x5046;
    static constexpr u16 s_address_voice2_sound_end = 0x5049;
    static constexpr u16 s_address_voice2_waveform = 0x504a;
    static constexpr u16 s_address_voice3_sound_beginning = 0x504b;
    static constexpr u16 s_address_voice3_sound_end = 0x504e;
    static constexpr u16 s_address_voice3_waveform = 0x504f;
    static constexpr u16 s_address_voice1_frequency_beginning = 0x5050;
    static constexpr u16 s_address_voice1_frequency_end = 0x5054;
    static constexpr u16 s_address_voice1_volume = 0x5055;
    static constexpr u16 s_address_voice2_frequency_beginning = 0x5056;
    static constexpr u16 s_address_voice2_frequency_end = 0x5059;
    static constexpr u16 s_address_voice2_volume = 0x505a;
    static constexpr u16 s_address_voice3_frequency_beginning = 0x505b;
    static constexpr u16 s_address_voice3_frequency_end = 0x505e;
    static constexpr u16 s_address_voice3_volume = 0x505f;
    static constexpr u16 s_address_sprite_coords_beginning = 0x5060;
    static constexpr u16 s_address_sprite_coords_end = 0x506f;
    static constexpr u16 s_address_watchdog_beginning = 0x50c0;
    static constexpr u16 s_address_watchdog_end = 0x50ff;
    static constexpr u16 s_address_pacman_memory_end = 0x50ff;

    EmulatorMemory<u16, u8>& m_memory;

    bool m_is_sound_enabled;
    bool m_is_aux_board_enabled;
    bool m_is_screen_flipped;
    u8 m_dipswitches;

    std::vector<Voice> m_voices;

    // Is read by the CPU:
    u8 m_in0_read = s_initial_value_in0_read;
    u8 m_in1_read = s_initial_value_in1_read;

    // Is written by the CPU:
    u8 m_in0_write;

    void in0_write(u8 value);

    void flip_screen(u8 value);

    void is_sound_enabled(u8 value);

    void is_aux_board_enabled(u8 value);

    void dipswitches(Settings const& settings);

    void board_test(Settings const& settings);

    void cabinet_mode(Settings const& settings);

    void voice1_accumulator(u8 value, u16 address);

    void voice1_waveform(u8 value);

    void voice1_frequency(u8 frequency, u16 address);

    void voice1_volume(u8 volume);

    void voice2_accumulator(u8 value, u16 address);

    void voice2_waveform(u8 value);

    void voice2_frequency(u8 frequency, u16 address);

    void voice2_volume(u8 volume);

    void voice3_accumulator(u8 value, u16 address);

    void voice3_waveform(u8 value);

    void voice3_frequency(u8 frequency, u16 address);

    void voice3_volume(u8 volume);
};
}
