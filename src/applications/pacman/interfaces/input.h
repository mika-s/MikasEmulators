#pragma once

#include "applications/pacman/memory_mapped_io_for_pacman.h"
#include "key_observer.h"
#include "pacman/gui_io.h"
#include <memory>

namespace emu::applications::pacman {

class Input {
public:
    virtual ~Input() = default;

    Input(const Input&) = delete;
    auto operator=(const Input&) -> Input& = delete;
    Input(Input&&) = delete;
    auto operator=(Input&&) -> Input& = delete;

    virtual void read(GuiIo& gui_io, std::shared_ptr<MemoryMappedIoForPacman> memory_mapped_io) = 0;

    virtual void read_debug_only(GuiIo& gui_io) = 0;

    virtual void add_io_observer(KeyObserver& observer) = 0;

    virtual void remove_io_observer(KeyObserver* observer) = 0;

protected:
    Input() = default;
};
}
