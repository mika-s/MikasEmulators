#pragma once

#include "chips/trivial/synacor/usings.h"
#include "ui.h"
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace emu::applications::synacor {
class UiObserver;
}
namespace emu::debugger {
template<class A, class D, std::size_t B>
class DebugContainer;
}
namespace emu::debugger {
template<class A, std::size_t B>
class Debugger;
}
namespace emu::logging {
class Logger;
}

namespace emu::applications::synacor {

using emu::synacor::Address;
using emu::synacor::Data;

class TuiTerminal : public Ui {

public:
    void to_terminal(Data character) override;

    void from_terminal() override;

    void clear_terminal() override;

    void add_ui_observer(UiObserver& observer) override;

    void remove_ui_observer(UiObserver* observer) override;

    void update_screen(bool is_awaiting_input, std::string const& game_window_subtitle) override;

    void update_debug_only(bool is_awaiting_input) override;

    void attach_debugger(std::shared_ptr<Debugger<Address, 16>> debugger) override;

    void attach_debug_container(std::shared_ptr<DebugContainer<Address, Data, 16>> debug_container) override;

    void attach_logger(std::shared_ptr<Logger> logger) override;

private:
    std::vector<UiObserver*> m_ui_observers;

    void notify_ui_observers_about_input_from_terminal(Data input);
};
}
