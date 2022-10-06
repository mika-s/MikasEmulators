#ifndef MIKA_EMULATORS_CROSSCUTTING_GUI_FRAMEBUFFER_H
#define MIKA_EMULATORS_CROSSCUTTING_GUI_FRAMEBUFFER_H

#include <vector>
#include "color.h"

namespace emu::gui {
    class Framebuffer {

    public:
        Framebuffer(unsigned int height, unsigned int width, Color init_color);

        void set(unsigned int row, unsigned int col, Color value);

        std::vector<u32> to_output_vector();

    private:
        unsigned int m_height;
        unsigned int m_width;
        std::vector<std::vector<u32>> m_values;
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_GUI_FRAMEBUFFER_H
