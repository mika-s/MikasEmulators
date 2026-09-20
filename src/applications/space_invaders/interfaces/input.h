#pragma once

#include "applications/space_invaders/cpu_io.h"
#include "key_observer.h"
#include "space_invaders/gui_io.h"

namespace emu::applications::space_invaders {

class Input {
public:
    virtual ~Input() = default;

    Input(const Input&) = delete;
    auto operator=(const Input&) -> Input& = delete;
    Input(Input&&) = delete;
    auto operator=(Input&&) -> Input& = delete;

    virtual void read(CpuIo& cpu_io, GuiIo& gui_io) = 0;

    virtual void read_debug_only(GuiIo& gui_io) = 0;

    virtual void add_io_observer(KeyObserver& observer) = 0;

    virtual void remove_io_observer(KeyObserver* observer) = 0;

protected:
    Input() = default;
};
}
