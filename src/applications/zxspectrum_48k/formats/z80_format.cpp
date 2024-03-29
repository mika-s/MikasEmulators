#include "z80_format.h"
#include "applications/zxspectrum_48k/interfaces/format.h"
#include "applications/zxspectrum_48k/joystick_type.h"
#include "chips/z80/flags.h"
#include "chips/z80/interrupt_mode.h"
#include "chips/z80/manual_state.h"
#include "crosscutting/exceptions/unsupported_exception.h"
#include "crosscutting/memory/next_byte.h"
#include "crosscutting/memory/next_word.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include "crosscutting/util/file_util.h"
#include "crosscutting/util/string_util.h"
#include <fmt/core.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace emu::applications::zxspectrum_48k {

using emu::exceptions::UnsupportedException;
using emu::memory::NextByte;
using emu::memory::NextWord;
using emu::util::byte::high_byte;
using emu::util::byte::is_bit_set;
using emu::util::byte::low_byte;
using emu::util::byte::to_u16;
using emu::util::file::read_file_into_vector;
using emu::util::string::hexify;

Z80Format::Z80Format(std::string const& file_path)
    : m_file_path(file_path)
{
    m_raw_data.add(read_file_into_vector(file_path));
    parse();
}

std::string Z80Format::version_string()
{
    switch (m_version) {
    case Z80FormatVersion::v1:
        return "v1";
    case Z80FormatVersion::v2:
        return "v2";
    case Z80FormatVersion::v3:
        return "v3";
    default:
        throw std::invalid_argument("m_version has to be v1, v2 or v3.");
    }
}

std::string Z80Format::interrupt_string()
{
    switch (m_interrupt_mode) {
    case InterruptMode::ZERO:
        return "0";
    case InterruptMode::ONE:
        return "1";
    case InterruptMode::TWO:
        return "2";
    default:
        throw std::invalid_argument("m_interrupt_mode has to be between 0-2.");
    }
}

std::string Z80Format::synchronization_string()
{
    switch (m_synchronization) {
    case 0:
    case 2:
        return "Normal";
    case 1:
        return "High video synchronisation";
    case 3:
        return "Low video synchronisation";
    default:
        throw std::invalid_argument(fmt::format("m_synchronization cannot be {}. Has to be between 0-3.", m_synchronization));
    }
}

std::string Z80Format::mgt_type_string()
{
    switch (m_mgt_type) {
    case 0:
        return "Disciple+Epson";
    case 1:
        return "Disciple+HP";
    case 16:
        return "Plus D";
    default:
        throw std::invalid_argument(fmt::format("m_mgt_type cannot be {}. Has to be 0, 1 or 16.", m_mgt_type));
    }
}

void Z80Format::print_header()
{
    std::cout << "Header data for " << m_file_path << ":\n\n";
    std::cout << "Version: " << version_string() << "\n\n";

    std::cout << "Version 1 headers:\n";
    std::cout << "------------------\n";
    std::cout << "A:                     " << hexify(m_acc_reg) << "\n";
    std::cout << "F:                     " << hexify(m_version == Z80FormatVersion::v1 ? m_flag_reg.to_u8() : m_flag_reg_before_v2_modification.to_u8()) << "\n";
    std::cout << "BC:                    " << hexify(to_u16(m_b_reg, m_c_reg)) << "\n";
    std::cout << "HL:                    " << hexify(to_u16(m_h_reg, m_l_reg)) << "\n";
    std::cout << "PC:                    " << hexify(m_version == Z80FormatVersion::v1 ? m_pc : m_pc_before_v2_modification) << "\n";
    std::cout << "SP:                    " << hexify(m_sp) << "\n";
    std::cout << "I:                     " << hexify(m_i_reg) << "\n";
    std::cout << "R:                     " << hexify(m_r_reg) << "\n";
    std::cout << "Border color:          " << +m_border_color << "\n";
    std::cout << "Basic SamRom:          " << (m_is_basic_samrom_switched_in ? "true" : "false") << "\n";
    std::cout << "Compression:           " << (m_is_block_of_data_compressed ? "true" : "false") << "\n";
    std::cout << "DE:                    " << hexify(to_u16(m_d_reg, m_e_reg)) << "\n";
    std::cout << "BC':                   " << hexify(to_u16(m_b_p_reg, m_c_p_reg)) << "\n";
    std::cout << "DE':                   " << hexify(to_u16(m_d_p_reg, m_e_p_reg)) << "\n";
    std::cout << "HL':                   " << hexify(to_u16(m_h_p_reg, m_l_p_reg)) << "\n";
    std::cout << "A':                    " << hexify(m_acc_p_reg) << "\n";
    std::cout << "F':                    " << hexify(m_flag_p_reg.to_u8()) << "\n";
    std::cout << "IY:                    " << hexify(m_iy_reg) << "\n";
    std::cout << "IX:                    " << hexify(m_ix_reg) << "\n";
    std::cout << "IFF1:                  " << (m_iff1 ? "true" : "false") << "\n";
    std::cout << "IFF2:                  " << (m_iff2 ? "true" : "false") << "\n";
    std::cout << "Interrupt mode:        " << interrupt_string() << "\n";
    std::cout << "Issue 2 emulation:     " << (m_is_issue2_emulation ? "true" : "false") << "\n";
    std::cout << "Double intr. freq:     " << (m_is_double_interrupt_frequency ? "true" : "false") << "\n";
    std::cout << "Synchronization:       " << hexify(m_synchronization) << " (" << synchronization_string() << ")"
              << "\n";
    std::cout << "Joystick:              " << s_joystick_type_as_string.at(m_joystick_type) << "\n";

    if (m_version == Z80FormatVersion::v2 || m_version == Z80FormatVersion::v3) {
        std::cout << "\nVersion 2 headers:\n";
        std::cout << "------------------\n";
        std::cout << "F:                     " << hexify(m_flag_reg.to_u8()) << "\n";
        std::cout << "Length of header:      " << +m_length_of_additional_header_block << "\n";
        std::cout << "PC:                    " << hexify(m_pc) << "\n";
        std::cout << "Hardware mode:         " << s_hardware_mode_as_string.at(m_hardware_mode) << "\n";
        std::cout << "Byte 35:               " << hexify(m_byte_35) << "\n";
        std::cout << "Byte 36:               " << hexify(m_byte_36) << "\n";
        std::cout << "R reg emulation:       " << (m_is_r_reg_emulation_on ? "true" : "false") << "\n";
        std::cout << "LDIR emulation:        " << (m_is_ldir_emulation_on ? "true" : "false") << "\n";
        std::cout << "AY sound in use:       " << (m_is_ay_sound_in_use ? "true" : "false") << "\n";
        std::cout << "Fuller Audio Box:      " << (m_is_fuller_audio_box_emulation ? "true" : "false") << "\n";
        std::cout << "Modifying hardware:    " << (m_is_modifying_hardware ? "true" : "false") << "\n";
        std::cout << "Last OUT 0xfffd:       " << hexify(m_last_out_0xfffd) << "\n";
        for (std::size_t i = 0; i < s_number_of_sound_registers; ++i) {
            if (i < 10) {
                std::cout << "Sound chip " << i << ":          " << hexify(m_sound_chip_registers[i]) << "\n";
            } else {
                std::cout << "Sound chip " << i << ":         " << hexify(m_sound_chip_registers[i]) << "\n";
            }
        }
    }

    if (m_version == Z80FormatVersion::v3) {
        std::cout << "\nVersion 3 headers:\n";
        std::cout << "------------------\n";
        std::cout << "Low T state counter:   " << hexify(m_low_T_state_counter) << "\n";
        std::cout << "High T state counter:  " << hexify(m_high_T_state_counter) << "\n";
        std::cout << "Flag byte Spectator:   " << hexify(m_flag_byte_spectator) << "\n";
        std::cout << "MGT ROM paged:         " << (m_is_mgt_rom_paged ? "true" : "false") << "\n";
        std::cout << "Multiface ROM paged:   " << (m_is_multiface_rom_paged ? "true" : "false") << "\n";
        std::cout << "0-8192:                " << (m_is_0_to_8192_rom ? "ROM" : "RAM") << "\n";
        std::cout << "8192-16383:            " << (m_is_8192_to_16383_rom ? "ROM" : "RAM") << "\n";
        for (std::size_t i = 0; i < s_number_of_keyboard_mappings; ++i) {
            std::cout << "KB map for UD joy " << i << ":   " << hexify(m_keyboard_mappings_for_ud_joystick[i]) << "\n";
        }
        std::cout << "KB label left:         " << high_byte(m_ascii_keyboard_mapping[0]) << low_byte(m_ascii_keyboard_mapping[0]) << "\n";
        std::cout << "KB label right:        " << high_byte(m_ascii_keyboard_mapping[1]) << low_byte(m_ascii_keyboard_mapping[1]) << "\n";
        std::cout << "KB label down:         " << high_byte(m_ascii_keyboard_mapping[2]) << low_byte(m_ascii_keyboard_mapping[2]) << "\n";
        std::cout << "KB label up:           " << high_byte(m_ascii_keyboard_mapping[3]) << low_byte(m_ascii_keyboard_mapping[3]) << "\n";
        std::cout << "KB label fire:         " << high_byte(m_ascii_keyboard_mapping[4]) << low_byte(m_ascii_keyboard_mapping[4]) << "\n";
        std::cout << "MGT type:              " << hexify(m_mgt_type) << " (" << mgt_type_string() << ")"
                  << "\n";
        std::cout << "Discp. inh. btn stat:  " << hexify(m_disciple_inhibit_button_status) << "\n";
        std::cout << "Discp. inh. flag:      " << hexify(m_disciple_inhibit_flag) << "\n";
        if (m_length_of_additional_header_block == 55) {
            std::cout << "Last OUT 0x1ffd:    " << hexify(m_last_out_0x1ffd) << "\n";
        }
    }
}

ManualState Z80Format::to_cpu_state()
{
    return {
        .m_iff1 = m_iff1,
        .m_iff2 = m_iff2,
        .m_sp = m_sp,
        .m_pc = m_pc,
        .m_acc_reg = m_acc_reg,
        .m_acc_p_reg = m_acc_p_reg,
        .m_b_reg = m_b_reg,
        .m_b_p_reg = m_b_p_reg,
        .m_c_reg = m_c_reg,
        .m_c_p_reg = m_c_p_reg,
        .m_d_reg = m_d_reg,
        .m_d_p_reg = m_d_p_reg,
        .m_e_reg = m_e_reg,
        .m_e_p_reg = m_e_p_reg,
        .m_h_reg = m_h_reg,
        .m_h_p_reg = m_h_p_reg,
        .m_l_reg = m_l_reg,
        .m_l_p_reg = m_l_p_reg,
        .m_ix_reg = m_ix_reg,
        .m_iy_reg = m_iy_reg,
        .m_i_reg = m_i_reg,
        .m_r_reg = m_r_reg,
        .m_flag_reg = m_flag_reg,
        .m_flag_p_reg = m_flag_p_reg,
        .m_interrupt_mode = m_interrupt_mode
    };
}

void Z80Format::to_memory(EmulatorMemory<u16, u8>& memory)
{
    if (m_version == Z80FormatVersion::v1) {
        read_block_v1(memory);
    } else {
        while (m_byte_counter < m_raw_data.size()) {
            read_block_v2(memory);
        }
    }
}

void Z80Format::read_block_v1(EmulatorMemory<u16, u8>& memory)
{
    std::vector<u8> output;

    while (m_byte_counter < m_raw_data.size() - 4) {
        const u8 byte = get_next_byte();

        if (m_is_block_of_data_compressed && byte == 0xed && m_raw_data.read(m_byte_counter) == 0xed) {
            get_next_byte();
            u8 repetitions = get_next_byte();
            const u8 value = get_next_byte();
            while (repetitions-- > 0) {
                output.push_back(value);
            }
        } else {
            output.push_back(byte);
        }
    }

    const u8 eb1 = get_next_byte();
    const u8 eb2 = get_next_byte();
    const u8 eb3 = get_next_byte();
    const u8 eb4 = get_next_byte();

    if (!(eb1 == 0x00 && eb2 == 0xed && eb3 == 0xed && eb4 == 0x00)) {
        throw std::invalid_argument(
            fmt::format(
                "The four final bytes were not 0x00 0xed 0xed 0x00, but {} {} {} {}",
                hexify(eb1), hexify(eb2), hexify(eb3), hexify(eb4)));
    }

    if (output.size() != 0xc000) {
        throw std::invalid_argument(
            fmt::format(
                "Output size is wrong when reading z80 format. Was {} but should be 0xc000",
                hexify(static_cast<u16>(output.size()))));
    }

    for (u16 address = 0x4000; address < 0xffff; ++address) {
        memory.write(address, output[address - 0x4000]);
    }
}

void Z80Format::read_block_v2(EmulatorMemory<u16, u8>& memory)
{
    std::vector<u8> output;

    //    std::cout << "Reading a block with:\n";
    const u16 length_of_compressed_data = get_next_word();
    const u8 page_number = get_next_byte();
    //    std::cout << "\tLength: " << hexify(length_of_compressed_data) << "\n";
    //    std::cout << "\tPage number: " << hexify(page_number) << "\n\n";

    u16 offset;
    switch (page_number) {
    case 4:
        offset = 0x8000;
        break;
    case 5:
        offset = 0xc000;
        break;
    case 8:
        offset = 0x4000;
        break;
    default:
        throw std::invalid_argument("Page has to be 4, 5 or 8.");
    }

    // We disregard the compression bool in the v1 header when using v2 block parsing.
    bool is_compressed = length_of_compressed_data != 0xffff;

    int i = 0;
    while (i++ < length_of_compressed_data) {
        const u8 byte = get_next_byte();

        if (is_compressed && byte == 0xed && m_raw_data.read(m_byte_counter) == 0xed) {
            get_next_byte();
            u8 repetitions = get_next_byte();
            const u8 value = get_next_byte();
            i += 3;
            while (repetitions-- > 0) {
                output.push_back(value);
            }
        } else {
            output.push_back(byte);
        }
    }

    if (output.size() != 0x4000) {
        throw std::invalid_argument(
            fmt::format(
                "Output size is wrong when reading z80 format. Was {} but should be 0x4000",
                hexify(static_cast<u16>(output.size()))));
    }

    for (std::size_t j = offset, k = 0; k < output.size(); ++j, ++k) {
        memory.write(j, output[k]);
    }
}

void Z80Format::parse()
{
    if (m_raw_data.size() < s_header_size_v1) {
        throw std::invalid_argument("The file is too small to be a z80 file.");
    }

    parse_v1();

    if (m_pc == 0) {
        parse_v2();
    }
    if (m_version == Z80FormatVersion::v3) {
        parse_v3();
    }
}

void Z80Format::parse_v1()
{
    m_version = Z80FormatVersion::v1;

    m_byte_counter = 0;

    m_acc_reg = get_next_byte();
    m_flag_reg.from_u8(get_next_byte());
    m_c_reg = get_next_byte();
    m_b_reg = get_next_byte();
    m_l_reg = get_next_byte();
    m_h_reg = get_next_byte();
    m_pc = get_next_word();
    m_sp = get_next_word();
    m_i_reg = get_next_byte();
    m_r_reg = get_next_byte() & 0b01111111;

    u8 misc_byte_1 = get_next_byte();
    if (misc_byte_1 == 255) { // For compatibility reasons.
        misc_byte_1 = 1;
    }
    m_r_reg += (misc_byte_1 & 0b00000001) << 7;
    m_border_color = (misc_byte_1 & 0b00001110) >> 1;
    m_is_basic_samrom_switched_in = is_bit_set(misc_byte_1, 4);
    m_is_block_of_data_compressed = is_bit_set(misc_byte_1, 5);

    m_e_reg = get_next_byte();
    m_d_reg = get_next_byte();
    m_c_p_reg = get_next_byte();
    m_b_p_reg = get_next_byte();
    m_e_p_reg = get_next_byte();
    m_d_p_reg = get_next_byte();
    m_l_p_reg = get_next_byte();
    m_h_p_reg = get_next_byte();
    m_acc_p_reg = get_next_byte();
    m_flag_p_reg.from_u8(get_next_byte());
    m_iy_reg = get_next_word();
    m_ix_reg = get_next_word();
    m_iff1 = get_next_byte() > 0;
    m_iff2 = get_next_byte() > 0;

    const u8 misc_byte_2 = get_next_byte();
    m_interrupt_mode = parse_interrupt_mode(misc_byte_2 & 0b00000011);
    m_is_issue2_emulation = is_bit_set(misc_byte_2, 2);
    m_is_double_interrupt_frequency = is_bit_set(misc_byte_2, 3);
    m_synchronization = misc_byte_2 & 0b00110000 >> 4;
    m_joystick_type = parse_joystick_type(misc_byte_2 & 0b11000000 >> 6);
}

void Z80Format::parse_v2()
{
    m_flag_reg_before_v2_modification = m_flag_reg;
    m_flag_reg.from_u8(m_flag_reg.to_u8() & 0b11001111);
    m_length_of_additional_header_block = get_next_word();

    if (m_length_of_additional_header_block == s_header_size_additional_v2) {
        m_version = Z80FormatVersion::v2;
    } else if (m_length_of_additional_header_block == s_header_size_additional_v3_1
        || m_length_of_additional_header_block == s_header_size_additional_v3_2) {
        m_version = Z80FormatVersion::v3;
    } else {
        throw std::invalid_argument(
            fmt::format("Invalid header length: {}. Must be {} for v2 or {}/{} for v3.",
                m_length_of_additional_header_block, s_header_size_additional_v2,
                s_header_size_additional_v3_1, s_header_size_additional_v3_2));
    }

    m_pc_before_v2_modification = m_pc;
    m_pc = get_next_word();
    m_hardware_mode = parse_hardware_mode(get_next_byte());

    if (m_hardware_mode != HardwareMode::_48k && m_hardware_mode != HardwareMode::_48k_If1) {
        throw UnsupportedException(
            fmt::format("Only hardware modes {} and {} are supported, but {} was provided",
                s_hardware_mode_as_string.at(HardwareMode::_48k),
                s_hardware_mode_as_string.at(HardwareMode::_48k_If1),
                s_hardware_mode_as_string.at(m_hardware_mode)));
    }

    m_byte_35 = get_next_byte();
    m_byte_36 = get_next_byte();

    u8 misc_byte3 = get_next_byte();
    m_is_r_reg_emulation_on = is_bit_set(misc_byte3, 0);
    m_is_ldir_emulation_on = is_bit_set(misc_byte3, 1);
    m_is_ay_sound_in_use = is_bit_set(misc_byte3, 2);
    m_is_fuller_audio_box_emulation = m_is_ay_sound_in_use && is_bit_set(misc_byte3, 6);
    m_is_modifying_hardware = is_bit_set(misc_byte3, 7);

    m_last_out_0xfffd = get_next_byte();

    for (std::size_t i = 0; i < s_number_of_sound_registers; ++i) {
        m_sound_chip_registers.push_back(get_next_byte());
    }
}

void Z80Format::parse_v3()
{
    m_low_T_state_counter = get_next_word();
    m_high_T_state_counter = get_next_byte();
    m_flag_byte_spectator = get_next_byte();
    m_is_mgt_rom_paged = get_next_byte() == 0xff;
    if (m_is_mgt_rom_paged) {
        throw UnsupportedException("MGT ROM paging");
    }

    m_is_multiface_rom_paged = get_next_byte() == 0xff;
    if (m_is_mgt_rom_paged) {
        throw UnsupportedException("Multiface ROM paging");
    }

    m_is_0_to_8192_rom = get_next_byte() == 0xff;
    m_is_8192_to_16383_rom = get_next_byte() == 0xff;

    for (std::size_t i = 0; i < s_number_of_keyboard_mappings; ++i) {
        m_keyboard_mappings_for_ud_joystick.push_back(get_next_word());
    }
    for (std::size_t i = 0; i < s_number_of_keyboard_mappings; ++i) {
        m_ascii_keyboard_mapping.push_back(get_next_word());
    }

    m_mgt_type = get_next_byte();
    m_disciple_inhibit_button_status = get_next_byte();
    m_disciple_inhibit_flag = get_next_byte();
    if (m_length_of_additional_header_block == 55) {
        m_last_out_0x1ffd = get_next_byte();
    }
}

InterruptMode Z80Format::parse_interrupt_mode(u8 raw_interrupt_mode)
{
    switch (raw_interrupt_mode) {
    case 0:
        return InterruptMode::ZERO;
    case 1:
        return InterruptMode::ONE;
    case 2:
        return InterruptMode::TWO;
    default:
        throw std::invalid_argument(fmt::format("Invalid interrupt mode: {}", raw_interrupt_mode));
    }
}

JoystickType Z80Format::parse_joystick_type(u8 raw_joystick_type)
{
    switch (raw_joystick_type) {
    case 0:
        return JoystickType::Cursor_Protek_AGF;
    case 1:
        return JoystickType::Kempston;
    case 2:
        if (m_version == Z80FormatVersion::v3) {
            return JoystickType::UserDefined;
        } else {
            return JoystickType::Sinclair_2_Left;
        }
    case 3:
        return JoystickType::Sinclair_2_Right;
    default:
        throw std::invalid_argument(fmt::format("Invalid joystick type: {}", raw_joystick_type));
    }
}

HardwareMode Z80Format::parse_hardware_mode(u8 raw_hardware_mode)
{
    switch (m_version) {
    case Z80FormatVersion::v2:
        switch (raw_hardware_mode) {
        case 0:
            return HardwareMode::_48k;
        case 1:
            return HardwareMode::_48k_If1;
        case 2:
            return HardwareMode::SamRam;
        case 3:
            return HardwareMode::_128k;
        case 4:
            return HardwareMode::_128k_If1;
        default:
            throw std::invalid_argument(fmt::format("Invalid hardware mode for v2: {}", raw_hardware_mode));
        }
    case Z80FormatVersion::v3:
        switch (raw_hardware_mode) {
        case 0:
            return HardwareMode::_48k;
        case 1:
            return HardwareMode::_48k_If1;
        case 2:
            return HardwareMode::SamRam;
        case 3:
            return HardwareMode::_48k_MGT;
        case 4:
            return HardwareMode::_128k;
        case 5:
            return HardwareMode::_128k_If1;
        case 6:
            return HardwareMode::_128k_MGT;
        default:
            throw std::invalid_argument(fmt::format("Invalid hardware mode for v2: {}", raw_hardware_mode));
        }
    default:
        throw std::invalid_argument("Cannot only parse hardware mode in v2 or v3");
    }
}

u8 Z80Format::get_next_byte()
{
    const NextByte next_byte = {
        .farg = m_raw_data.read(m_byte_counter++)
    };

    return next_byte.farg;
}

u16 Z80Format::get_next_word()
{
    const NextWord next_word = {
        .farg = m_raw_data.read(m_byte_counter++),
        .sarg = m_raw_data.read(m_byte_counter++)
    };

    return to_u16(next_word.sarg, next_word.farg);
}

}
