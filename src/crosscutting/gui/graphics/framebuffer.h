#pragma once

#include "crosscutting/typedefs.h"
#include <vector>

namespace emu::gui {
class Color;
}

namespace emu::gui {
class Framebuffer {

public:
    Framebuffer(unsigned int height, unsigned int width, Color init_color);

    void set(unsigned int row, unsigned int col, Color value);

    void flip_vertical();

    void clear();

    std::vector<u32> to_output_vector();

    unsigned int height() const;

    unsigned int width() const;

private:
    unsigned int m_height;
    unsigned int m_width;
    std::vector<std::vector<u32>> m_values;
};
}
