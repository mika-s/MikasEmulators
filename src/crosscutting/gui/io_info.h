#ifndef MIKA_EMULATORS_CROSSCUTTING_GUI_IO_INFO_H
#define MIKA_EMULATORS_CROSSCUTTING_GUI_IO_INFO_H

#include <string>
#include <tuple>
#include <unordered_map>
#include "8080/debug_container.h"

namespace emu::util::gui {
    class IoInfo {
    public:
        IoInfo();

        void attach_debug_container(emu::cpu8080::DebugContainer &debug_container);

        void draw(const char *title, bool *p_open = nullptr);

    private:
        emu::cpu8080::DebugContainer m_debug_container;
        std::unordered_map<std::string, std::tuple<int, std::uint8_t>> saved_values;

        static constexpr int cycles_to_keep_value = 300;
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_GUI_IO_INFO_H
