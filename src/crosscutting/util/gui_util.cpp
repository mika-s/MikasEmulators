#include <stdexcept>
#include "gui_util.h"

namespace emu::util::gui {

    const std::vector<std::pair<int, int>> number_to_pixels(
            unsigned int number,
            unsigned int row_offset,
            unsigned int col_offset
    ) {
        switch (number) {
            case 0x0:
                return {
                        {row_offset + 0, col_offset + 0},
                        {row_offset + 0, col_offset + 1},
                        {row_offset + 0, col_offset + 2},
                        {row_offset + 1, col_offset + 0},
                        {row_offset + 1, col_offset + 2},
                        {row_offset + 2, col_offset + 0},
                        {row_offset + 2, col_offset + 2},
                        {row_offset + 3, col_offset + 0},
                        {row_offset + 3, col_offset + 2},
                        {row_offset + 4, col_offset + 0},
                        {row_offset + 4, col_offset + 1},
                        {row_offset + 4, col_offset + 2}
                };
            case 0x1:
                return {
                        {row_offset + 0, col_offset + 2},
                        {row_offset + 1, col_offset + 2},
                        {row_offset + 2, col_offset + 2},
                        {row_offset + 3, col_offset + 2},
                        {row_offset + 4, col_offset + 2}
                };
            case 0x2:
                return {
                        {row_offset + 0, col_offset + 0},
                        {row_offset + 0, col_offset + 1},
                        {row_offset + 0, col_offset + 2},
                        {row_offset + 1, col_offset + 2},
                        {row_offset + 2, col_offset + 0},
                        {row_offset + 2, col_offset + 1},
                        {row_offset + 2, col_offset + 2},
                        {row_offset + 3, col_offset + 0},
                        {row_offset + 4, col_offset + 0},
                        {row_offset + 4, col_offset + 1},
                        {row_offset + 4, col_offset + 2}
                };
            case 0x3:
                return {
                        {row_offset + 0, col_offset + 0},
                        {row_offset + 0, col_offset + 1},
                        {row_offset + 0, col_offset + 2},
                        {row_offset + 1, col_offset + 2},
                        {row_offset + 2, col_offset + 1},
                        {row_offset + 2, col_offset + 2},
                        {row_offset + 3, col_offset + 2},
                        {row_offset + 4, col_offset + 0},
                        {row_offset + 4, col_offset + 1},
                        {row_offset + 4, col_offset + 2}
                };
            case 0x4:
                return {
                        {row_offset + 0, col_offset + 0},
                        {row_offset + 0, col_offset + 2},
                        {row_offset + 1, col_offset + 0},
                        {row_offset + 1, col_offset + 2},
                        {row_offset + 2, col_offset + 0},
                        {row_offset + 2, col_offset + 1},
                        {row_offset + 2, col_offset + 2},
                        {row_offset + 3, col_offset + 2},
                        {row_offset + 4, col_offset + 2}
                };
            case 0x5:
                return {
                        {row_offset + 0, col_offset + 0},
                        {row_offset + 0, col_offset + 1},
                        {row_offset + 0, col_offset + 2},
                        {row_offset + 1, col_offset + 0},
                        {row_offset + 2, col_offset + 0},
                        {row_offset + 2, col_offset + 1},
                        {row_offset + 2, col_offset + 2},
                        {row_offset + 3, col_offset + 2},
                        {row_offset + 4, col_offset + 0},
                        {row_offset + 4, col_offset + 1},
                        {row_offset + 4, col_offset + 2}
                };
            case 0x6:
                return {
                        {row_offset + 0, col_offset + 0},
                        {row_offset + 0, col_offset + 1},
                        {row_offset + 0, col_offset + 2},
                        {row_offset + 1, col_offset + 2},
                        {row_offset + 2, col_offset + 0},
                        {row_offset + 2, col_offset + 2},
                        {row_offset + 3, col_offset + 0},
                        {row_offset + 3, col_offset + 2},
                        {row_offset + 4, col_offset + 0},
                        {row_offset + 4, col_offset + 1},
                        {row_offset + 4, col_offset + 2}
                };
            case 0x7:
                return {
                        {row_offset + 0, col_offset + 0},
                        {row_offset + 0, col_offset + 1},
                        {row_offset + 0, col_offset + 2},
                        {row_offset + 1, col_offset + 2},
                        {row_offset + 2, col_offset + 2},
                        {row_offset + 3, col_offset + 2},
                        {row_offset + 4, col_offset + 2}
                };
            case 0x8:
                return {
                        {row_offset + 0, col_offset + 0},
                        {row_offset + 0, col_offset + 1},
                        {row_offset + 0, col_offset + 2},
                        {row_offset + 1, col_offset + 0},
                        {row_offset + 1, col_offset + 2},
                        {row_offset + 2, col_offset + 0},
                        {row_offset + 2, col_offset + 1},
                        {row_offset + 2, col_offset + 2},
                        {row_offset + 3, col_offset + 0},
                        {row_offset + 3, col_offset + 2},
                        {row_offset + 4, col_offset + 0},
                        {row_offset + 4, col_offset + 1},
                        {row_offset + 4, col_offset + 2}
                };
            case 0x9:
                return {
                        {row_offset + 0, col_offset + 0},
                        {row_offset + 0, col_offset + 1},
                        {row_offset + 0, col_offset + 2},
                        {row_offset + 1, col_offset + 0},
                        {row_offset + 1, col_offset + 2},
                        {row_offset + 2, col_offset + 0},
                        {row_offset + 2, col_offset + 1},
                        {row_offset + 2, col_offset + 2},
                        {row_offset + 3, col_offset + 2},
                        {row_offset + 4, col_offset + 2}
                };
            case 0xa:
                return {
                        {row_offset + 0, col_offset + 0},
                        {row_offset + 0, col_offset + 1},
                        {row_offset + 0, col_offset + 2},
                        {row_offset + 1, col_offset + 0},
                        {row_offset + 1, col_offset + 2},
                        {row_offset + 2, col_offset + 0},
                        {row_offset + 2, col_offset + 2},
                        {row_offset + 3, col_offset + 0},
                        {row_offset + 3, col_offset + 2},
                        {row_offset + 4, col_offset + 0},
                        {row_offset + 4, col_offset + 2}
                };
            case 0xb:
                return {
                        {row_offset + 0, col_offset + 0},
                        {row_offset + 0, col_offset + 1},
                        {row_offset + 1, col_offset + 0},
                        {row_offset + 1, col_offset + 2},
                        {row_offset + 2, col_offset + 0},
                        {row_offset + 2, col_offset + 1},
                        {row_offset + 2, col_offset + 2},
                        {row_offset + 3, col_offset + 0},
                        {row_offset + 3, col_offset + 2},
                        {row_offset + 4, col_offset + 0},
                        {row_offset + 4, col_offset + 1}
                };
            case 0xc:
                return {
                        {row_offset + 0, col_offset + 0},
                        {row_offset + 0, col_offset + 1},
                        {row_offset + 0, col_offset + 2},
                        {row_offset + 1, col_offset + 0},
                        {row_offset + 2, col_offset + 0},
                        {row_offset + 3, col_offset + 0},
                        {row_offset + 4, col_offset + 0},
                        {row_offset + 4, col_offset + 1},
                        {row_offset + 4, col_offset + 2}
                };
            case 0xd:
                return {
                        {row_offset + 0, col_offset + 0},
                        {row_offset + 0, col_offset + 1},
                        {row_offset + 1, col_offset + 0},
                        {row_offset + 1, col_offset + 2},
                        {row_offset + 2, col_offset + 0},
                        {row_offset + 2, col_offset + 2},
                        {row_offset + 3, col_offset + 0},
                        {row_offset + 3, col_offset + 2},
                        {row_offset + 4, col_offset + 0},
                        {row_offset + 4, col_offset + 1}
                };
            case 0xe:
                return {
                        {row_offset + 0, col_offset + 0},
                        {row_offset + 0, col_offset + 1},
                        {row_offset + 0, col_offset + 2},
                        {row_offset + 1, col_offset + 0},
                        {row_offset + 2, col_offset + 0},
                        {row_offset + 2, col_offset + 1},
                        {row_offset + 2, col_offset + 2},
                        {row_offset + 3, col_offset + 0},
                        {row_offset + 4, col_offset + 0},
                        {row_offset + 4, col_offset + 1},
                        {row_offset + 4, col_offset + 2}
                };
            case 0xf:
                return {
                        {row_offset + 0, col_offset + 0},
                        {row_offset + 0, col_offset + 1},
                        {row_offset + 0, col_offset + 2},
                        {row_offset + 1, col_offset + 0},
                        {row_offset + 2, col_offset + 0},
                        {row_offset + 2, col_offset + 1},
                        {row_offset + 2, col_offset + 2},
                        {row_offset + 3, col_offset + 0},
                        {row_offset + 4, col_offset + 0}
                };
            default:
                throw std::invalid_argument("Number cannot be larger than 0xf");
        }
    }
}
