#include "memory_mapped_io_for_game_boy.h"
#include "boot_rom.h"
#include "chips/z80/util.h"
#include "crosscutting/memory/emulator_memory.h"
#include "interrupts.h"
#include "lcd.h"
#include "lcd_control.h"
#include "lcd_status.h"
#include "settings.h"
#include "timer.h"
#include <array>
#include <iostream>
#include <utility>

namespace emu::applications::game_boy {

using emu::util::byte::is_bit_set;
using emu::util::byte::set_bit;
using emu::util::byte::unset_bit;
using emu::z80::set_bit_in_memory;
using emu::z80::unset_bit_in_memory;

MemoryMappedIoForGameBoy::MemoryMappedIoForGameBoy(
    EmulatorMemory<u16, u8>& memory,
    std::shared_ptr<Timer> timer,
    std::shared_ptr<Lcd> lcd,
    [[maybe_unused]] Settings settings)
    : m_memory(memory)
    , m_timer(std::move(timer))
    , m_lcd(std::move(lcd))
{
}

/**
 * Called when the CPU writes to memory
 *
 * @param address is the address in memory to write to
 * @param value is the value that should be written to memory
 */
void MemoryMappedIoForGameBoy::write(u16 address, u8 value)
{
    if (address <= s_address_rom_end) {
        // Writes to ROM are ignored.
    } else if (s_address_tile_ram_beginning <= address && address <= s_address_tile_ram_end) {
        m_memory.direct_write(address, value);
    } else if (s_address_background_map_beginning <= address && address <= s_address_background_map_end) {
        m_memory.direct_write(address, value);
    } else if (s_address_cartridge_ram_beginning <= address && address <= s_address_cartridge_ram_end) {
        m_memory.direct_write(address, value);
    } else if (s_address_working_ram_beginning <= address && address <= s_address_working_ram_end) {
        m_memory.direct_write(address, value);
    } else if (s_address_echo_ram_beginning <= address && address <= s_address_echo_ram_end) {
        // Writes to echo RAM are ignored.
    } else if (s_address_object_attribute_memory_beginning <= address && address <= s_address_object_attribute_memory_end) {
        m_memory.direct_write(address, value);
    } else if (s_address_unused_memory_beginning <= address && address <= s_address_unused_memory_end) {
        // Writes to unused memory region are ignored.
    } else if (s_address_io_ports_beginning <= address && address <= s_address_io_ports_end) {
        m_memory.direct_write(address, value);
    } else if (s_address_high_ram_beginning <= address && address <= s_address_high_ram_end) {
        m_memory.direct_write(address, value);
    } else if (address == s_address_serial_transfer_data) {
        m_memory.direct_write(address, value);
    } else if (address == s_address_serial_transfer_control) {
        if (value == s_blargg_serial_output_token) {
            std::cout << m_memory.direct_read(address);
            m_memory.direct_write(address, 0);
        } else {
            m_memory.direct_write(address, value);
        }
    } else if (address == s_address_timer_divider_register) {
        m_timer->reset_divider();
    } else if (address == s_address_timer_counter) {
        m_timer->counter(value);
    } else if (address == s_address_timer_modulo) {
        m_timer->modulo(value);
    } else if (address == s_address_timer_control) {
        m_timer->control(value);
    } else if (address == s_address_interrupt_f_register) {
        m_if = value;
        m_memory.direct_write(address, value);
    } else if (address == s_address_lcd_control) {
        m_lcd->lcd_control().update_from_memory(value);
    } else if (address == s_address_lcd_status) {
        m_lcd->lcd_status().update_from_memory(value);
    } else if (address == s_address_lcd_viewport_y_position) {
        m_lcd->m_scy = value;
    } else if (address == s_address_lcd_viewport_x_position) {
        m_lcd->m_scx = value;
    } else if (address == s_address_lcd_y_coordinate) {
        m_memory.direct_write(address, 0);
    } else if (address == s_address_lcd_y_coordinate_compare) {
        m_lcd->m_lyc = value;
    } else if (address == s_address_oam_dma) {
        dma_transfer(value);
    } else if (address == s_address_lcd_window_y_position) {
        m_lcd->m_wy = value;
    } else if (address == s_address_lcd_window_x_position) {
        m_lcd->m_wx = value;
    } else if (address == s_address_boot_rom_active) {
        m_is_boot_rom_active = value != 0;
        m_memory.direct_write(address, value);
    } else if (address == s_address_interrupt_enabled_register) {
        m_ie = value > 1;
        m_memory.direct_write(address, value);
    } else {
        m_memory.direct_write(address, value);
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
    if (address <= s_address_rom_end) {
        if (address <= s_address_boot_rom_end && m_is_boot_rom_active) {
            return s_boot_rom[address];
        } else {
            return m_memory.direct_read(address);
        }
    } else if (s_address_tile_ram_beginning <= address && address <= s_address_tile_ram_end) {
        return m_memory.direct_read(address);
    } else if (s_address_background_map_beginning <= address && address <= s_address_background_map_end) {
        return m_memory.direct_read(address);
    } else if (s_address_cartridge_ram_beginning <= address && address <= s_address_cartridge_ram_end) {
        return m_memory.direct_read(address);
    } else if (s_address_working_ram_beginning <= address && address <= s_address_working_ram_end) {
        return m_memory.direct_read(address);
    } else if (s_address_echo_ram_beginning <= address && address <= s_address_echo_ram_end) {
        // Reads from echo RAM returns 0.
        return 0;
    } else if (s_address_object_attribute_memory_beginning <= address && address <= s_address_object_attribute_memory_end) {
        return m_memory.direct_read(address);
    } else if (s_address_unused_memory_beginning <= address && address <= s_address_unused_memory_end) {
        // Writes to unused memory region are ignored.
        return 0;
    } else if (s_address_io_ports_beginning <= address && address <= s_address_io_ports_end) {
        return m_memory.direct_read(address);
    } else if (s_address_high_ram_beginning <= address && address <= s_address_high_ram_end) {
        return m_memory.direct_read(address);
    } else if (address == s_address_joypad) {
        return m_p1;
    } else if (address == s_address_serial_transfer_data) {
        return m_memory.direct_read(address);
    } else if (address == s_address_serial_transfer_control) {
        return m_memory.direct_read(address);
    } else if (address == s_address_timer_divider_register) {
        return m_timer->divider();
    } else if (address == s_address_timer_counter) {
        return m_timer->counter();
    } else if (address == s_address_timer_modulo) {
        return m_timer->modulo();
    } else if (address == s_address_timer_control) {
        return m_timer->control();
    } else if (address == s_address_interrupt_f_register) {
        return m_memory.direct_read(address);
    } else if (address == s_address_lcd_control) {
        return m_lcd->lcd_control().to_u8();
    } else if (address == s_address_lcd_status) {
        return m_lcd->lcd_status().to_u8();
    } else if (address == s_address_lcd_viewport_y_position) {
        return m_lcd->m_scy;
    } else if (address == s_address_lcd_viewport_x_position) {
        return m_lcd->m_scx;
    } else if (address == s_address_lcd_y_coordinate) {
        return m_lcd->m_ly;
    } else if (address == s_address_lcd_y_coordinate_compare) {
        return m_lcd->m_lyc;
    } else if (address == s_address_lcd_window_y_position) {
        return m_lcd->m_wy;
    } else if (address == s_address_lcd_window_x_position) {
        return m_lcd->m_wx;
    } else if (address == s_address_interrupt_enabled_register) {
        return m_memory.direct_read(address);
    } else {
        return m_memory.direct_read(address);
    }
}

bool MemoryMappedIoForGameBoy::ie()
{
    return m_ie;
}

void MemoryMappedIoForGameBoy::p1(unsigned int bit_number, bool is_setting)
{
    if (is_setting) {
        set_bit(m_p1, bit_number);
    } else {
        unset_bit(m_p1, bit_number);
    }
}

u8 MemoryMappedIoForGameBoy::p1() const
{
    return m_p1;
}

u8 MemoryMappedIoForGameBoy::if_()
{
    return m_if;
}

void MemoryMappedIoForGameBoy::interrupt(Interrupts interrupt)
{
    u8 value = read(s_address_interrupt_f_register);

    switch (interrupt) {
    case VBLANK:
        set_bit(value, s_interrupt_bit_vblank);
        break;
    case LCD:
        set_bit(value, s_interrupt_bit_lcd);
        break;
    case TIMER:
        set_bit(value, s_interrupt_bit_timer);
        break;
    case JOYPAD:
        set_bit(value, s_interrupt_bit_joypad);
        break;
    }

    write(s_address_interrupt_f_register, value);
}

void MemoryMappedIoForGameBoy::reset_interrupt(Interrupts interrupt)
{
    u8 value = read(s_address_interrupt_f_register);

    switch (interrupt) {
    case VBLANK:
        unset_bit(value, s_interrupt_bit_vblank);
        break;
    case LCD:
        unset_bit(value, s_interrupt_bit_lcd);
        break;
    case TIMER:
        unset_bit(value, s_interrupt_bit_timer);
        break;
    case JOYPAD:
        unset_bit(value, s_interrupt_bit_joypad);
        break;
    }

    write(s_address_interrupt_f_register, value);
}

bool MemoryMappedIoForGameBoy::is_boot_rom_active() const
{
    return m_is_boot_rom_active;
}

void MemoryMappedIoForGameBoy::dma_transfer(u8 value)
{
    for (u16 dest_address = s_address_object_attribute_memory_beginning, src_address = value << 8;
         dest_address <= s_address_object_attribute_memory_end;
         ++dest_address, ++src_address) {
        m_memory.direct_write(dest_address, m_memory.read(src_address));
    }
}

}
