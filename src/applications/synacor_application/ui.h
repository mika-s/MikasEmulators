#pragma once

#include "chips/trivial/synacor/usings.h"
#include "crosscutting/debugging/debug_container.h"
#include "crosscutting/debugging/debugger.h"
#include "crosscutting/gui/graphics/color.h"
#include "crosscutting/gui/graphics/framebuffer.h"
#include "crosscutting/logging/logger.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include "interfaces/ui_observer.h"

namespace emu::applications::synacor {

using emu::debugger::DebugContainer;
using emu::debugger::Debugger;
using emu::logging::Logger;
using emu::synacor::Address;
using emu::synacor::Data;
using emu::util::byte::is_bit_set;
using emu::util::byte::to_u32;

class Ui {
public:
    Ui() = default;

    virtual ~Ui() = default;

    virtual void to_terminal(Data character) = 0;

    virtual void from_terminal() = 0;

    virtual void clear_terminal() = 0;

    virtual void add_ui_observer(UiObserver& observer) = 0;

    virtual void remove_ui_observer(UiObserver* observer) = 0;

    virtual void update_screen(bool is_awaiting_input, std::string const& game_window_subtitle) = 0;

    virtual void update_debug_only(bool is_awaiting_input) = 0;

    virtual void attach_debugger(std::shared_ptr<Debugger<Address, 16>> debugger) = 0;

    virtual void attach_debug_container(std::shared_ptr<DebugContainer<Address, Data, 16>> debug_container) = 0;

    virtual void attach_logger(std::shared_ptr<Logger> logger) = 0;

protected:
    static constexpr float s_scale = 4.0;
    static constexpr int s_width = 224;
    static constexpr int s_height = 256;
    static constexpr int s_scaled_width = static_cast<int>(s_scale * static_cast<float>(s_width));
    static constexpr int s_scaled_height = static_cast<int>(s_scale * static_cast<float>(s_height));
};
}
