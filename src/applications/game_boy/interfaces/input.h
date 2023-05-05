#pragma once

#include "applications/game_boy/memory_mapped_io_for_game_boy.h"
#include "game_boy/gui_io.h"
#include "key_observer.h"
#include <memory>

namespace emu::applications::game_boy {

class Input {
public:
    virtual ~Input() = default;

    virtual void read(GuiIo& gui_io, std::shared_ptr<MemoryMappedIoForGameBoy> memory_mapped_io) = 0;

    virtual void read_debug_only(GuiIo& gui_io) = 0;

    virtual void add_io_observer(KeyObserver& observer) = 0;

    virtual void remove_io_observer(KeyObserver* observer) = 0;
};
}
