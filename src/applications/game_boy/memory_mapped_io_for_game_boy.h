#pragma once

#include "crosscutting/memory/memory_mapped_io.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include "lcd_control.h"

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

class MemoryMappedIoForGameBoy : public MemoryMappedIo<u16, u8> {
public:
    explicit MemoryMappedIoForGameBoy(EmulatorMemory<u16, u8>& memory, Settings settings);

    void write(u16 address, u8 value) override;

    u8 read(u16 address) override;

    void p1(unsigned int bit_number, bool is_setting);

    [[nodiscard]] u8 p1() const;

    bool is_interrupt_enabled();

    [[nodiscard]] LcdControl lcd_control() const;

private:
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
    static constexpr u16 s_address_io_register = 0xff00;
    static constexpr u16 s_address_interrupt_f_register = 0xff0f;
    static constexpr u16 s_address_lcd_control = 0xff40;
    static constexpr u16 s_address_interrupt_enabled_register = 0xffff;

    EmulatorMemory<u16, u8>& m_memory;

    bool m_if { false };
    bool m_ie { false };

    LcdControl m_lcd_control;

    u8 m_p1 { 0xff };
};
}
