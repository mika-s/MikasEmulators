#ifndef MIKA_EMULATORS_CROSSCUTTING_GUI_CPU_INFO_H
#define MIKA_EMULATORS_CROSSCUTTING_GUI_CPU_INFO_H

#include <functional>
#include <imgui.h>
#include <string>
#include <tuple>
#include <vector>
#include "8080/debug_container.h"

namespace emu::util::gui {
    class CpuInfo {
    public:
        CpuInfo();

        void attach_debug_container(emu::cpu8080::DebugContainer &debug_container);

        void draw(const char *title, bool *p_open = nullptr);

    private:
        emu::cpu8080::DebugContainer debug_container;

        void add_register(const std::string& name, const std::function<std::uint8_t()>& value_retriever);
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_GUI_CPU_INFO_H
