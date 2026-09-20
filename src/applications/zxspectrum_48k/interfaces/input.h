#pragma once

#include "key_observer.h"
#include "zxspectrum_48k/cpu_io.h"
#include "zxspectrum_48k/gui_io.h"

namespace emu::applications::zxspectrum_48k {

class Input {
public:
    virtual ~Input() = default;

    Input(const Input&) = delete;
    auto operator=(const Input&) -> Input& = delete;
    Input(Input&&) = delete;
    auto operator=(Input&&) -> Input& = delete;

    virtual void read(CpuIo& cpu_io, GuiIo& gui_io) = 0;

    virtual void read_debug_only(CpuIo& cpu_io, GuiIo& gui_io) = 0;

    virtual void add_io_observer(KeyObserver& observer) = 0;

    virtual void remove_io_observer(KeyObserver* observer) = 0;

protected:
    Input() = default;
};
}
