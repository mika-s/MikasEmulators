#pragma once

#include <utility>
#include <vector>

namespace emu::util::gui {

auto number_to_pixels(
    unsigned int number,
    unsigned int row_offset,
    unsigned int col_offset
) -> std::vector<std::pair<int, int>>;
}
