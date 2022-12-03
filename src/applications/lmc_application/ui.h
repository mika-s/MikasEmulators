#ifndef MIKA_EMULATORS_APPLICATIONS_LMC_APPLICATION_UI_H
#define MIKA_EMULATORS_APPLICATIONS_LMC_APPLICATION_UI_H

#include "chips/trivial/lmc/interfaces/gui_observer.h"
#include "crosscutting/debugging/debug_container.h"
#include "crosscutting/debugging/debugger.h"
#include "crosscutting/gui/graphics/color.h"
#include "crosscutting/gui/graphics/framebuffer.h"
#include "crosscutting/logging/logger.h"
#include "crosscutting/misc/run_status.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"

namespace emu::applications::lmc {

    using emu::debugger::DebugContainer;
    using emu::debugger::Debugger;
    using emu::lmc::GuiObserver;
    using emu::logging::Logger;
    using emu::misc::RunStatus;
    using emu::util::byte::is_bit_set;
    using emu::util::byte::to_u32;

    class Ui {
    public:
        Ui() = default;

        virtual ~Ui() = default;

        virtual void to_terminal() = 0;

        virtual void add_gui_observer(GuiObserver &observer) = 0;

        virtual void remove_gui_observer(GuiObserver *observer) = 0;

        virtual void update_screen(RunStatus run_status) = 0;

        virtual void update_debug_only() = 0;

        virtual void attach_debugger(std::shared_ptr<Debugger> debugger) = 0;

        virtual void attach_debug_container(DebugContainer &debug_container) = 0;

        virtual void attach_logger(std::shared_ptr<Logger> logger) = 0;

    protected:
        static constexpr float scale = 4.0;
        static constexpr int width = 224;
        static constexpr int height = 256;
        static constexpr int scaled_width = static_cast<int>(scale * static_cast<float>(width));
        static constexpr int scaled_height = static_cast<int>(scale * static_cast<float>(height));
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_LMC_APPLICATION_UI_H
