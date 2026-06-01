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

    virtual auto is_initialized() -> bool
    {
        return true;
    }

    void set(std::size_t row, std::size_t col, Color value);

    void flip_horizontal();

    void flip_vertical();

    void map_to_framebuffer(Framebuffer& framebuffer, int origin_row, int origin_col) const;

    [[nodiscard]] auto size() const -> std::size_t;

private:
    std::vector<std::vector<Color>> m_values;
    std::size_t m_height;
    std::size_t m_width;

    [[nodiscard]] auto get(std::size_t row, std::size_t col) const -> Color;
};

class UninitializedSprite : public Sprite {
public:
    UninitializedSprite();

    auto is_initialized() -> bool override
    {
        return false;
    }
};
}
