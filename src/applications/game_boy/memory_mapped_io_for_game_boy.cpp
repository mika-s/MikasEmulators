#include "memory_mapped_io_for_game_boy.h"
#include "chips/z80/util.h"
#include "crosscutting/memory/emulator_memory.h"
#include "lcd_control.h"
#include "settings.h"
#include "timer.h"
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
    [[maybe_unused]] Settings settings)
    : m_memory(memory)
    , m_timer(std::move(timer))
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
    } else if (address == s_address_timer_divider_register) {
        m_timer->reset_divider();
    } else if (address == s_address_timer_counter) {
        m_timer->counter(value);
    } else if (address == s_address_timer_modulo) {
        m_timer->modulo(value);
    } else if (address == s_address_timer_control) {
        m_timer->control(value);
    } else if (address == s_address_interrupt_f_register) {
        m_if = value > 1;
    } else if (address == s_address_lcd_control) {
        m_lcd_control.update_from_memory(value);
        m_memory.direct_write(address, value);
    } else if (address == s_address_interrupt_enabled_register) {
        m_ie = value > 1;
    } else {
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
    // TODO: When reaching 0x100, the boot ROM area is no longer pointing to the boot ROM.
    if (address <= s_address_rom_end) {
        return m_memory.direct_read(address);
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
    } else if (address == s_address_timer_divider_register) {
        return m_timer->divider();
    } else if (address == s_address_timer_counter) {
        return m_timer->counter();
    } else if (address == s_address_timer_modulo) {
        return m_timer->modulo();
    } else if (address == s_address_timer_control) {
        return m_timer->control();
    } else if (address == s_address_interrupt_f_register) {
        return m_if;
    } else if (address == s_address_interrupt_enabled_register) {
        return m_ie;
    } else {
        return 0;
    }
}

bool MemoryMappedIoForGameBoy::is_interrupt_enabled()
{
    return m_if;
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

LcdControl MemoryMappedIoForGameBoy::lcd_control() const
{
    return m_lcd_control;
}

}
