#pragma once

#include "crosscutting/memory/memory_mapped_io.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include "interrupts.h"
#include <memory>

namespace emu::applications::game_boy {
class Lcd;
class Settings;
class Timer;
}
namespace emu::memory {
template<class A, class D>
class EmulatorMemory;
}

namespace emu::applications::game_boy {

using emu::memory::EmulatorMemory;
using emu::memory::MemoryMappedIo;
using emu::util::byte::low_nibble;

class MemoryMappedIoForGameBoy : public MemoryMappedIo<u16, u8> {
public:
    explicit MemoryMappedIoForGameBoy(
        EmulatorMemory<u16, u8>& memory,
        std::shared_ptr<Timer> timer,
        std::shared_ptr<Lcd> lcd,
        Settings settings);

    void write(u16 address, u8 value) override;

    u8 read(u16 address) override;

    void p1_button_keys(unsigned int bit_number, bool is_setting);

    void p1_direction_keys(unsigned int bit_number, bool is_setting);

    [[nodiscard]] u8 p1() const;

    [[nodiscard]] bool ie();

    [[nodiscard]] u8 if_();

    void interrupt(Interrupts interrupt);

    void reset_interrupt(Interrupts interrupt);

    [[nodiscard]] bool is_boot_rom_active() const;

private:
    static constexpr u16 s_interrupt_bit_vblank = 0;
    static constexpr u16 s_interrupt_bit_lcd = 1;
    static constexpr u16 s_interrupt_bit_timer = 2;
    static constexpr u16 s_interrupt_bit_joypad = 3;

    static constexpr u16 s_bit_number_select_button_keys = 5;
    static constexpr u16 s_bit_number_select_direction_keys = 4;

    static constexpr u16 s_address_boot_rom_end = 0x00ff;
    static constexpr u16 s_address_rom_end = 0x7fff;
    static constexpr u16 s_address_tile_ram_beginning = 0x8000;
    static constexpr u16 s_address_tile_ram_end = 0x97ff;
    static constexpr u16 s_address_background_map_beginning = 0x9800;
    static constexpr u16 s_address_background_map_end = 0x9fff;
    static constexpr u16 s_address_cartridge_ram_beginning = 0xa000;
    static constexpr u16 s_address_cartridge_ram_end = 0xbfff;
    static constexpr u16 s_address_working_ram_beginning = 0xc000;
    static constexpr u16 s_address_working_ram_end = 0xdfff;
    static constexpr u16 s_address_echo_ram_beginning = 0xe000;
    static constexpr u16 s_address_echo_ram_end = 0xfdff;
    static constexpr u16 s_address_object_attribute_memory_beginning = 0xfe00;
    static constexpr u16 s_address_object_attribute_memory_end = 0xfe9f;
    static constexpr u16 s_address_unused_memory_beginning = 0xfea0;
    static constexpr u16 s_address_unused_memory_end = 0xfeff;
    static constexpr u16 s_address_io_ports_beginning = 0xff00;
    static constexpr u16 s_address_io_ports_end = 0xff7f;
    static constexpr u16 s_address_high_ram_beginning = 0xff80;
    static constexpr u16 s_address_high_ram_end = 0xfffe;
    static constexpr u16 s_address_game_boy_memory_end = 0x50ff;

    static constexpr u8 s_blargg_serial_output_token = 0x81;

    /************/
    /* IO ports */
    /************/

    static constexpr u16 s_address_joypad = 0xff00; // P1/JOYP

    // Serial
    static constexpr u16 s_address_serial_transfer_data = 0xff01;    // SB
    static constexpr u16 s_address_serial_transfer_control = 0xff02; // SC

    // Timer
    static constexpr u16 s_address_timer_divider_register = 0xff04; // DIV
    static constexpr u16 s_address_timer_counter = 0xff05;          // TIMA
    static constexpr u16 s_address_timer_modulo = 0xff06;           // TMA
    static constexpr u16 s_address_timer_control = 0xff07;          // TAC

    static constexpr u16 s_address_interrupt_f_register = 0xff0f; // IF

    // Sound
    static constexpr u16 s_address_sound_ch_1_sweep = 0xff10;                       // NR10
    static constexpr u16 s_address_sound_ch_1_length_timer_and_duty_cycle = 0xff11; // NR11
    static constexpr u16 s_address_sound_ch_1_volume_and_envelope = 0xff12;         // NR12
    static constexpr u16 s_address_sound_ch_1_wavelength_low = 0xff13;              // NR13
    static constexpr u16 s_address_sound_ch_1_wavelength_high_and_control = 0xff14; // NR14
    static constexpr u16 s_address_sound_ch_2_length_timer_and_duty_cycle = 0xff16; // NR21
    static constexpr u16 s_address_sound_ch_2_volume_and_envelope = 0xff17;         // NR22
    static constexpr u16 s_address_sound_ch_2_wavelength_low = 0xff18;              // NR23
    static constexpr u16 s_address_sound_ch_2_wavelength_high_and_control = 0xff19; // NR24
    static constexpr u16 s_address_sound_ch_3_dac_enable = 0xff1a;                  // NR30
    static constexpr u16 s_address_sound_ch_3_length_timer = 0xff1b;                // NR31
    static constexpr u16 s_address_sound_ch_3_output_level = 0xff1c;                // NR32
    static constexpr u16 s_address_sound_ch_3_wavelength_low = 0xff1d;              // NR33
    static constexpr u16 s_address_sound_ch_3_wavelength_high_and_control = 0xff1e; // NR34
    static constexpr u16 s_address_sound_ch_4_length_timer = 0xff20;                // NR41
    static constexpr u16 s_address_sound_ch_4_volume_and_envelope = 0xff21;         // NR42
    static constexpr u16 s_address_sound_ch_4_frequency_and_randomness = 0xff22;    // NR43
    static constexpr u16 s_address_sound_ch_4_control = 0xff23;                     // NR44
    static constexpr u16 s_address_sound_master_volume_and_vin_panning = 0xff24;    // NR50
    static constexpr u16 s_address_sound_panning = 0xff25;                          // NR51
    static constexpr u16 s_address_sound_on_off = 0xff26;                           // NR52

    static constexpr u16 s_address_sound_wave_ram_beginning = 0xff30; // Wave RAM beginning
    static constexpr u16 s_address_sound_wave_ram_end = 0xff3f;       // Wave RAM end

    // LCD
    static constexpr u16 s_address_lcd_control = 0xff40;              // LCDC
    static constexpr u16 s_address_lcd_status = 0xff41;               // STAT
    static constexpr u16 s_address_lcd_viewport_y_position = 0xff42;  // SCY
    static constexpr u16 s_address_lcd_viewport_x_position = 0xff43;  // SCX
    static constexpr u16 s_address_lcd_y_coordinate = 0xff44;         // LY
    static constexpr u16 s_address_lcd_y_coordinate_compare = 0xff45; // LYC
    static constexpr u16 s_address_oam_dma = 0xff46;                  // OAM DMA
    static constexpr u16 s_address_lcd_window_y_position = 0xff4a;    // WY
    static constexpr u16 s_address_lcd_window_x_position = 0xff4b;    // WX

    static constexpr u16 s_address_boot_rom_active = 0xff50;
    static constexpr u16 s_address_interrupt_enabled_register = 0xffff; // IE

    EmulatorMemory<u16, u8>& m_memory;
    std::shared_ptr<Timer> m_timer;
    std::shared_ptr<Lcd> m_lcd;

    bool m_is_boot_rom_active { true };

    u8 m_if { 0 };
    bool m_ie { false };

    u8 m_p1_button_keys { 0xff };
    u8 m_p1_direction_keys { 0xff };
    bool m_is_reading_direction_keys { false };

    void dma_transfer(u8 value);
};
}
