#pragma once

#include "crosscutting/misc/run_status.h"
#include <string>

namespace emu::lmc {

using emu::misc::RunStatus;

class UiObserver {
public:
    virtual ~UiObserver() = default;

    virtual void run_status_changed(RunStatus new_status) = 0;

    virtual void debug_mode_changed(bool is_in_debug_mode) = 0;

    virtual void source_code_changed(std::string const& source) = 0;

    virtual void assemble_and_load_request() = 0;

    virtual void input_from_terminal(Data input) = 0;
};
}
