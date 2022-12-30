#pragma once

#include "crosscutting/typedefs.h"
#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

namespace emu::applications::zxspectrum_48k {
class CpuIo;
}

namespace emu::applications::zxspectrum_48k {

class KeyboardPane {
public:
    void attach_cpu_io(CpuIo const* cpu_io);

    void draw(char const* title, bool* p_open = nullptr);

private:
    static constexpr std::size_t s_rows = 8;
    static constexpr std::size_t s_cols = 6;

    static const inline std::unordered_map<std::size_t, u16> s_index_to_address = {
        { 0, 0xfefe },
        { 1, 0xfdfe },
        { 2, 0xfbfe },
        { 3, 0xf7fe },
        { 4, 0xeffe },
        { 5, 0xdffe },
        { 6, 0xbffe },
        { 7, 0x7ffe }
    };

    static const inline std::vector<std::vector<std::string>> s_keyboard_labels = {
        { "0xfefe", "SHIFT", "Z", "X", "C", "V" },
        { "0xfdfe", "A", "S", "D", "F", "G" },
        { "0xfbfe", "Q", "W", "E", "R", "T" },
        { "0xf7fe", "1", "2", "3", "4", "5" },
        { "0xeffe", "0", "9", "8", "7", "6" },
        { "0xdffe", "P", "O", "I", "U", "Y" },
        { "0xbffe", "ENTER", "L", "K", "J", "H" },
        { "0x7ffe", "SPACE", "SYM", "M", "N", "B" }
    };

    CpuIo const* m_cpu_io;
    bool m_is_cpu_io_set { false };
};
}
