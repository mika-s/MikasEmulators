#pragma once

#include "chips/trivial/lmc/out_type.h"
#include "chips/trivial/lmc/usings.h"
#include "crosscutting/misc/run_status.h"
#include "lmc_application/terminal_input_state.h"
#include "ui.h"
#include <cstddef>
#include <memory>
#include <vector>

namespace emu::debugger {
template<class A, class D, std::size_t B>
class DebugContainer;
}
namespace emu::debugger {
template<class A, std::size_t B>
class Debugger;
}
namespace emu::lmc {
class UiObserver;
}
namespace emu::logging {
class Logger;
}

namespace emu::applications::lmc {

using emu::lmc::Address;
using emu::lmc::Data;
using emu::lmc::OutType;
using emu::misc::RunStatus;

class TuiTerminal : public Ui {

public:
    void to_terminal(Data acc_reg, OutType out_type) override;

    void from_terminal() override;

    void clear_terminal() override;

    void add_ui_observer(UiObserver& observer) override;

    void remove_ui_observer(UiObserver* observer) override;

    void update_screen(RunStatus run_status, TerminalInputState terminal_input_state) override;

    void update_debug_only(TerminalInputState terminal_input_state) override;

    void attach_debugger(std::shared_ptr<Debugger<Address, 10>> debugger) override;

    void attach_debug_container(std::shared_ptr<DebugContainer<Address, Data, 10>> debug_container) override;

    void attach_logger(std::shared_ptr<Logger> logger) override;

private:
    std::vector<UiObserver*> m_ui_observers;

    void notify_ui_observers_about_input_from_terminal(Data input);
};
}
