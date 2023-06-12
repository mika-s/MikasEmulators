#pragma once

#include "crosscutting/memory/memory_mapped_io.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include "interrupts.h"
#include "lcd_control.h"
#include "lcd_status.h"
#include <array>
#include <memory>

namespace emu::applications::game_boy {
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
        Settings settings);

    void write(u16 address, u8 value) override;

    u8 read(u16 address) override;

    void p1(unsigned int bit_number, bool is_setting);

    [[nodiscard]] u8 p1() const;

    [[nodiscard]] bool ie();

    [[nodiscard]] u8 if_();

    void interrupt(Interrupts interrupt);

    void reset_interrupt(Interrupts interrupt);

    [[nodiscard]] LcdControl lcd_control() const;

    [[nodiscard]] LcdStatus lcd_status() const;

    [[nodiscard]] bool is_boot_rom_active() const;

    void increment_scanline();

    void reset_scanline();

private:
    static constexpr std::array<u8, 256> s_boot_rom = {
        0x31, 0xFE, 0xFF, 0xAF, 0x21, 0xFF, 0x9F, 0x32, 0xCB, 0x7C, 0x20, 0xFB, 0x21, 0x26, 0xFF, 0x0E,
        0x11, 0x3E, 0x80, 0x32, 0xE2, 0x0C, 0x3E, 0xF3, 0xE2, 0x32, 0x3E, 0x77, 0x77, 0x3E, 0xFC, 0xE0,
        0x47, 0x11, 0x04, 0x01, 0x21, 0x10, 0x80, 0x1A, 0xCD, 0x95, 0x00, 0xCD, 0x96, 0x00, 0x13, 0x7B,
        0xFE, 0x34, 0x20, 0xF3, 0x11, 0xD8, 0x00, 0x06, 0x08, 0x1A, 0x13, 0x22, 0x23, 0x05, 0x20, 0xF9,
        0x3E, 0x19, 0xEA, 0x10, 0x99, 0x21, 0x2F, 0x99, 0x0E, 0x0C, 0x3D, 0x28, 0x08, 0x32, 0x0D, 0x20,
        0xF9, 0x2E, 0x0F, 0x18, 0xF3, 0x67, 0x3E, 0x64, 0x57, 0xE0, 0x42, 0x3E, 0x91, 0xE0, 0x40, 0x04,
        0x1E, 0x02, 0x0E, 0x0C, 0xF0, 0x44, 0xFE, 0x90, 0x20, 0xFA, 0x0D, 0x20, 0xF7, 0x1D, 0x20, 0xF2,
        0x0E, 0x13, 0x24, 0x7C, 0x1E, 0x83, 0xFE, 0x62, 0x28, 0x06, 0x1E, 0xC1, 0xFE, 0x64, 0x20, 0x06,
        0x7B, 0xE2, 0x0C, 0x3E, 0x87, 0xE2, 0xF0, 0x42, 0x90, 0xE0, 0x42, 0x15, 0x20, 0xD2, 0x05, 0x20,
        0x4F, 0x16, 0x20, 0x18, 0xCB, 0x4F, 0x06, 0x04, 0xC5, 0xCB, 0x11, 0x17, 0xC1, 0xCB, 0x11, 0x17,
        0x05, 0x20, 0xF5, 0x22, 0x23, 0x22, 0x23, 0xC9, 0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B,
        0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D, 0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E,
        0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99, 0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC,
        0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E, 0x3C, 0x42, 0xB9, 0xA5, 0xB9, 0xA5, 0x42, 0x3C,
        0x21, 0x04, 0x01, 0x11, 0xA8, 0x00, 0x1A, 0x13, 0xBE, 0x00, 0x00, 0x23, 0x7D, 0xFE, 0x34, 0x20,
        0xF5, 0x06, 0x19, 0x78, 0x86, 0x23, 0x05, 0x20, 0xFB, 0x86, 0x00, 0x00, 0x3E, 0x01, 0xE0, 0x50
    };

    static constexpr u16 s_interrupt_bit_vblank = 0;
    static constexpr u16 s_interrupt_bit_lcd = 1;
    static constexpr u16 s_interrupt_bit_timer = 2;
    static constexpr u16 s_interrupt_bit_joypad = 3;

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

    bool m_is_boot_rom_active { true };

    u8 m_if { 0 };
    bool m_ie { false };

    LcdControl m_lcd_control;
    LcdStatus m_lcd_status;
    u8 m_ly { 0 };
    u8 m_lyc { 0 };
    u8 m_scy { 0 };
    u8 m_scx { 0 };
    u8 m_wy { 0 };
    u8 m_wx { 0 };

    u8 m_p1 { 0xff };

    void dma_transfer(u8 value);
};
}
