#pragma once

#include "crosscutting/misc/run_status.h"
#include "io_observer.h"

namespace emu::applications::lmc {

using emu::misc::RunStatus;

class Input {
public:
    virtual ~Input() = default;

    virtual void read(RunStatus& run_status) = 0;

    virtual void read_debug_only(RunStatus& run_status) = 0;

    virtual void add_io_observer(IoObserver& observer) = 0;

    virtual void remove_io_observer(IoObserver* observer) = 0;
};
}
