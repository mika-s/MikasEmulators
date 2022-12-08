#pragma once

#include "applications/pacman/memory_mapped_io_for_pacman.h"
#include "crosscutting/misc/run_status.h"
#include "io_observer.h"
#include <memory>

namespace emu::applications::pacman {

using emu::misc::RunStatus;

class Input {
public:
    virtual ~Input() = default;

    virtual void read(RunStatus& run_status, std::shared_ptr<MemoryMappedIoForPacman> memory_mapped_io) = 0;

    virtual void read_debug_only(RunStatus& run_status) = 0;

    virtual void add_io_observer(IoObserver& observer) = 0;

    virtual void remove_io_observer(IoObserver* observer) = 0;
};
}
