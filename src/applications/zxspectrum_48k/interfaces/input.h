#pragma once

#include "key_observer.h"
#include "zxspectrum_48k/gui_io.h"

namespace emu::applications::zxspectrum_48k {

class Input {
public:
    virtual ~Input() = default;

    virtual void read(GuiIo& gui_io) = 0;

    virtual void read_debug_only(GuiIo& gui_io) = 0;

    virtual void add_io_observer(KeyObserver& observer) = 0;

    virtual void remove_io_observer(KeyObserver* observer) = 0;
};
}
