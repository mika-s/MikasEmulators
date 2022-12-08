#include "tui_terminal.h"
#include "chips/trivial/lmc/usings.h"
#include "crosscutting/misc/uinteger.h"
#include "crosscutting/typedefs.h"
#include "lmc/interfaces/ui_observer.h"
#include "lmc/out_type.h"
#include "ui.h"
#include <algorithm>
#include <iostream>

namespace emu::applications::lmc {

using emu::lmc::OutType;

void TuiTerminal::to_terminal(Data acc_reg, OutType out_type)
{
    if (out_type == OutType::OUT) {
        std::cout << acc_reg.underlying() << "\n";
    } else {
        std::cout << static_cast<char>(acc_reg.underlying());
    }
}

void TuiTerminal::from_terminal()
{
    std::cout << "inp: " << std::flush;

    u16 number;
    std::cin >> number;
    notify_ui_observers_about_input_from_terminal(Data(number));
}

void TuiTerminal::clear_terminal()
{
}

void TuiTerminal::add_ui_observer(UiObserver& observer)
{
    m_ui_observers.push_back(&observer);
}

void TuiTerminal::remove_ui_observer(UiObserver* observer)
{
    m_ui_observers.erase(
        std::remove(m_ui_observers.begin(), m_ui_observers.end(), observer),
        m_ui_observers.end());
}

void TuiTerminal::attach_debugger([[maybe_unused]] std::shared_ptr<Debugger<Address, 10>> debugger)
{
}

void TuiTerminal::attach_debug_container([[maybe_unused]] std::shared_ptr<DebugContainer<Address, Data, 10>> debug_container)
{
}

void TuiTerminal::attach_logger([[maybe_unused]] std::shared_ptr<Logger> logger)
{
}

void TuiTerminal::update_screen([[maybe_unused]] RunStatus run_status, [[maybe_unused]] TerminalInputState terminal_input_state)
{
}

void TuiTerminal::update_debug_only([[maybe_unused]] TerminalInputState terminal_input_state)
{
}

void TuiTerminal::notify_ui_observers_about_input_from_terminal(Data input)
{
    for (UiObserver* observer : m_ui_observers) {
        observer->input_from_terminal(input);
    }
}
}
