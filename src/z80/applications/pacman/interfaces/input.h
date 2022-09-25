#ifndef MIKA_EMULATORS_Z80_APPLICATIONS_PACMAN_INTERFACES_INPUT_H
#define MIKA_EMULATORS_Z80_APPLICATIONS_PACMAN_INTERFACES_INPUT_H

#include "z80/run_status.h"
#include "z80/applications/pacman/memory_mapped_io.h"
#include "z80/applications/pacman/interfaces/io_observer.h"

namespace emu::z80::applications::pacman {

    class Input {
    public:
        virtual ~Input() = default;

        virtual void read(RunStatus &run_status, std::shared_ptr<MemoryMappedIo> memory_mapped_io) = 0;

        virtual void read_debug_only(RunStatus &run_status) = 0;

        virtual void add_io_observer(IoObserver &observer) = 0;

        virtual void remove_io_observer(IoObserver *observer) = 0;
    };
}

#endif //MIKA_EMULATORS_Z80_APPLICATIONS_PACMAN_INTERFACES_INPUT_H
