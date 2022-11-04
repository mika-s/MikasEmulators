#ifndef MIKA_EMULATORS_CROSSCUTTING_GUI_UTIL_H
#define MIKA_EMULATORS_CROSSCUTTING_GUI_UTIL_H

#include <utility>
#include <vector>

namespace emu::util::gui {

    const std::vector<std::pair<int, int>> number_to_pixels(
            unsigned int number,
            unsigned int row_offset,
            unsigned int col_offset
    );
}

#endif //MIKA_EMULATORS_CROSSCUTTING_GUI_UTIL_H
