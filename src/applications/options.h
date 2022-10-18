#ifndef MIKA_EMULATORS_APPLICATIONS_OPTIONS_H
#define MIKA_EMULATORS_APPLICATIONS_OPTIONS_H

#include <string>
#include <unordered_map>
#include <vector>
#include "applications/pacman/settings.h"
#include "applications/space_invaders/settings.h"
#include "crosscutting/gui/gui_type.h"

namespace emu::applications {

    using emu::gui::GuiType;

    class Options {
    public:
        explicit Options(std::vector<std::string> args);

        std::vector<std::string> args();

        GuiType gui_type();

        applications::pacman::Settings pacman_settings();

        applications::space_invaders::Settings space_invaders_settings();

    private:
        const std::string dipswitch_flag = "-d";
        const std::string gui_flag = "-g";

        std::vector<std::string> m_args;
        std::unordered_map<std::string, std::vector<std::string>> m_options;

        void parse_flag(const std::string &flag, std::vector<std::string> args, std::size_t arg_idx);
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_OPTIONS_H
