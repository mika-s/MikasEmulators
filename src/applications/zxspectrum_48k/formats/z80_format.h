#pragma once

#include "applications/zxspectrum_48k/interfaces/format.h"
#include "chips/z80/flags.h"
#include "chips/z80/interrupt_mode.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/memory/next_byte.h"
#include "crosscutting/memory/next_word.h"
#include "crosscutting/typedefs.h"
#include <cstddef>
#include <string>
#include <vector>

namespace emu::z80 {
struct ManualState;
}

namespace emu::applications::zxspectrum_48k {

using emu::memory::EmulatorMemory;
using emu::memory::NextByte;
using emu::memory::NextWord;
using emu::z80::Flags;
using emu::z80::InterruptMode;

enum class Z80FormatVersion {
    Unknown,
    v1,
    v2,
    v3
};

class Z80Format : public Format {
public:
    explicit Z80Format(std::string const& file_path);

    void print_header() override;

    ManualState to_cpu_state() override;

    void to_memory(EmulatorMemory<u16, u8>& memory) override;

private:
    static constexpr std::size_t s_header_size_v1 = 30;
    static constexpr std::size_t s_number_of_sound_registers = 16;
    static constexpr std::size_t s_number_of_keyboard_mappings = 5;

    Z80FormatVersion m_version { Z80FormatVersion::Unknown };

    std::string m_file_path;
    EmulatorMemory<u16, u8> m_raw_data;
    std::size_t m_byte_counter = { 0 };

    /**
     * Format description (https://worldofspectrum.org/faq/reference/z80format.htm):
     *
     * V1:
     *
     *        Offset  Length  Description
     *        ---------------------------
     *        0       1       A register
     *        1       1       F register
     *        2       2       BC register pair (LSB, i.e. C, first)
     *        4       2       HL register pair
     *        6       2       Program counter
     *        8       2       Stack pointer
     *        10      1       Interrupt register
     *        11      1       Refresh register (Bit 7 is not significant!)
     *        12      1       Bit 0  : Bit 7 of the R-register
     *                        Bit 1-3: Border colour
     *                        Bit 4  : 1=Basic SamRom switched in
     *                        Bit 5  : 1=Block of data is compressed
     *                        Bit 6-7: No meaning
     *        13      2       DE register pair
     *        15      2       BC' register pair
     *        17      2       DE' register pair
     *        19      2       HL' register pair
     *        21      1       A' register
     *        22      1       F' register
     *        23      2       IY register (Again LSB first)
     *        25      2       IX register
     *        27      1       Interrupt flipflop, 0=DI, otherwise EI
     *        28      1       IFF2 (not particularly important...)
     *        29      1       Bit 0-1: Interrupt mode (0, 1 or 2)
     *                        Bit 2  : 1=Issue 2 emulation
     *                        Bit 3  : 1=Double interrupt frequency
     *                        Bit 4-5: 1=High video synchronisation
     *                                 3=Low video synchronisation
     *                                 0,2=Normal
     *                        Bit 6-7: 0=Cursor/Protek/AGF joystick
     *                                 1=Kempston joystick
     *                                 2=Sinclair 2 Left joystick (or user
     *                                   defined, for version 3 .z80 files)
     *                                 3=Sinclair 2 Right joystick
     */
    u8 m_acc_reg;
    Flags m_flag_reg_before_v2_modification;
    Flags m_flag_reg;
    u8 m_b_reg;
    u8 m_c_reg;
    u8 m_h_reg;
    u8 m_l_reg;
    u16 m_pc_before_v2_modification;
    u16 m_pc;
    u16 m_sp;
    u8 m_i_reg;
    u8 m_r_reg;
    u8 m_border_color;
    bool m_is_basic_samrom_switched_in;
    bool m_is_block_of_data_compressed;
    u8 m_d_reg;
    u8 m_e_reg;
    u8 m_b_p_reg;
    u8 m_c_p_reg;
    u8 m_d_p_reg;
    u8 m_e_p_reg;
    u8 m_h_p_reg;
    u8 m_l_p_reg;
    u8 m_acc_p_reg;
    Flags m_flag_p_reg;
    u16 m_iy_reg;
    u16 m_ix_reg;
    bool m_iff1;
    bool m_iff2;
    InterruptMode m_interrupt_mode;
    bool m_is_issue2_emulation;           // Emulate an Issue 2 Spectrum: EAR bit is 1 when not toggling. The bit is 0 when not toggling on Issue 3.
    bool m_is_double_interrupt_frequency; // Interrupt twice as often
    u8 m_synchronization;                 // Remove flickering of moving characters in some programs
    u8 m_joystick_type;

    /**
     * V2 (in addition to the header above):
     *
     *        Offset  Length  Description
     *        ---------------------------
     *      * 30      2       Length of additional header block (see below)
     *      * 32      2       Program counter
     *      * 34      1       Hardware mode (see below)
     *      * 35      1       If in SamRam mode, bitwise state of 74ls259.
     *                        For example, bit 6=1 after an OUT 31,13 (=2*6+1)
     *                        If in 128 mode, contains last OUT to 0x7ffd
     *                        If in Timex mode, contains last OUT to 0xf4
     *      * 36      1       Contains 0xff if Interface I rom paged
     *                        If in Timex mode, contains last OUT to 0xff
     *      * 37      1       Bit 0: 1 if R register emulation on
     *                        Bit 1: 1 if LDIR emulation on
     *                        Bit 2: AY sound in use, even on 48K machines
     *                        Bit 6: (if bit 2 set) Fuller Audio Box emulation
     *                        Bit 7: Modify hardware (see below)
     *      * 38      1       Last OUT to port 0xfffd (soundchip register number)
     *      * 39      16      Contents of the sound chip registers
     */
    u16 m_length_of_additional_header_block;
    u8 m_hardware_mode;
    u8 m_byte_35;
    u8 m_byte_36;
    bool m_is_r_reg_emulation_on; // On: Real Z80 behavior. Off: Random number generator, except for bit 7, which stays the same.
    bool m_is_ldir_emulation_on;  // Emulate LDIR with an x86 instructions that does the same thing, but faster.
    bool m_is_ay_sound_in_use;
    bool m_is_fuller_audio_box_emulation;
    bool m_is_modifying_hardware;
    u8 m_last_out_0xfffd;
    std::vector<u8> m_sound_chip_registers;

    /**
     * V3 (in addition to the header above):
     *
     *        Offset  Length  Description
     *        ---------------------------
     *        55      2       Low T state counter
     *        57      1       Hi T state counter
     *        58      1       Flag byte used by Spectator (QL spec. emulator)
     *                        Ignored by Z80 when loading, zero when saving
     *        59      1       0xff if MGT Rom paged
     *        60      1       0xff if Multiface Rom paged. Should always be 0.
     *        61      1       0xff if 0-8191 is ROM, 0 if RAM
     *        62      1       0xff if 8192-16383 is ROM, 0 if RAM
     *        63      10      5 x keyboard mappings for user defined joystick
     *        73      10      5 x ASCII word: keys corresponding to mappings above
     *        83      1       MGT type: 0=Disciple+Epson,1=Disciple+HP,16=Plus D
     *        84      1       Disciple inhibit button status: 0=out, 0ff=in
     *        85      1       Disciple inhibit flag: 0=rom pageable, 0ff=not
     *     ** 86      1       Last OUT to port 0x1ffd
     */
    u16 m_low_T_state_counter;
    u8 m_high_T_state_counter;
    u8 m_flag_byte_spectator;
    bool m_is_mgt_rom_paged;
    bool m_is_multiface_rom_paged;
    bool m_is_0_to_8192_rom;
    bool m_is_8192_to_16383_rom;
    std::vector<u16> m_keyboard_mappings_for_ud_joystick;
    std::vector<u16> m_ascii_keyboard_mapping;
    u8 m_mgt_type;
    u8 m_disciple_inhibit_button_status; // Floppy disk interface. Inhibit: turn off
    u8 m_disciple_inhibit_flag;          // Floppy disk interface
    u8 m_last_out_0x1ffd;

    void read_block_v2(EmulatorMemory<u16, u8>& memory);

    void parse();

    void parse_v1();

    void parse_v2();

    void parse_v3();

    InterruptMode parse_interrupt_mode(u8 raw_interrupt_mode);

    u8 get_next_byte();

    u16 get_next_word();

    std::string version_string();

    std::string interrupt_string();

    std::string synchronization_string();

    std::string joystick_string();

    std::string mgt_type_string();
};

}
