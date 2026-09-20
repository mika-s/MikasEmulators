#pragma once

#include "applications/game_boy/gui_io.h"
#include "applications/game_boy/interfaces/interrupt_observer.h"
#include "applications/game_boy/interfaces/key_observer.h"
#include "applications/game_boy/memory_mapped_io_for_game_boy.h"
#include <memory>

namespace emu::applications::game_boy {

class Input {
public:
    virtual ~Input() = default;

    Input(const Input&) = delete;
    auto operator=(const Input&) -> Input& = delete;
    Input(Input&&) = delete;
    auto operator=(Input&&) -> Input& = delete;

    virtual void read(GuiIo& gui_io, std::shared_ptr<MemoryMappedIoForGameBoy> memory_mapped_io) = 0;

    virtual void read_debug_only(GuiIo& gui_io) = 0;

    virtual void add_io_observer(KeyObserver& observer) = 0;

    virtual void remove_io_observer(KeyObserver* observer) = 0;

    virtual void add_interrupt_observer(InterruptObserver& observer) = 0;

    virtual void remove_interrupt_observer(InterruptObserver* observer) = 0;

protected:
    Input() = default;
};
}
