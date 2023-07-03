#pragma once

#include "color.h"
#include <cstddef>
#include <vector>

namespace emu::gui {
class Framebuffer;
}

namespace emu::gui {

class Sprite {
public:
    Sprite(std::size_t height, std::size_t width);

    virtual ~Sprite() = default;

    virtual bool is_initialized()
    {
        return true;
    }

    void set(std::size_t row, std::size_t col, Color value);

    void flip_horizontal();

    void flip_vertical();

    void map_to_framebuffer(Framebuffer& framebuffer, int origin_row, int origin_col);

    std::size_t size() const;

private:
    std::vector<std::vector<Color>> m_values;
    std::size_t m_height;
    std::size_t m_width;

    Color get(std::size_t row, std::size_t col);
};

class UninitializedSprite : public Sprite {
public:
    UninitializedSprite();

    bool is_initialized() override
    {
        return false;
    }
};
}
