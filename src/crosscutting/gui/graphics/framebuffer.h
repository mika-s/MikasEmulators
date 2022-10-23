#ifndef MIKA_EMULATORS_CROSSCUTTING_GUI_GRAPHICS_FRAMEBUFFER_H
#define MIKA_EMULATORS_CROSSCUTTING_GUI_GRAPHICS_FRAMEBUFFER_H

#include <vector>
#include "color.h"
#include "crosscutting/typedefs.h"

namespace emu::gui {
    class Framebuffer {

    public:
        Framebuffer(unsigned int height, unsigned int width, Color init_color);

        void set(unsigned int row, unsigned int col, Color value);

        void flip_vertical();

        void clear();

        std::vector<u32> to_output_vector();

        unsigned int height();

        unsigned int width();

    private:
        unsigned int m_height;
        unsigned int m_width;
        std::vector<std::vector<u32>> m_values;
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_GUI_GRAPHICS_FRAMEBUFFER_H
