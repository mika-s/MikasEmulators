#pragma once

#include "color.h"
#include <cstddef>
#include <vector>

namespace emu::gui {
class Framebuffer;
}

namespace emu::gui {

class Tile {
public:
    Tile(std::size_t height, std::size_t width);

    virtual ~Tile() = default;

    virtual auto is_initialized() -> bool
    {
        return true;
    }

    void set(std::size_t row, std::size_t col, Color value);

    void map_to_framebuffer(
        Framebuffer& framebuffer,
        unsigned int origin_row,
        unsigned int origin_col) const;

    [[nodiscard]] auto size() const -> std::size_t;

private:
    std::vector<std::vector<Color>> m_values;
    std::size_t m_height;
    std::size_t m_width;

    [[nodiscard]] auto get(std::size_t row, std::size_t col) const -> Color;
};

class UninitializedTile : public Tile {
public:
    UninitializedTile();

    auto is_initialized() -> bool override
    {
        return false;
    }
};
}
