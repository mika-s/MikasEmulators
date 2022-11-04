#ifndef MIKA_EMULATORS_APPLICATIONS_OPTIONS_H
#define MIKA_EMULATORS_APPLICATIONS_OPTIONS_H

#include "crosscutting/gui/gui_type.h"
#include <cstddef>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace emu::applications {

    using emu::gui::GuiType;

    class Options {
    public:
        explicit Options(std::vector<std::string> args, std::string short_executable_name);

        std::vector<std::string> args();

        std::string short_executable_name();

        GuiType gui_type(const std::function<void(const std::string &)> &print_usage);

        bool is_asking_for_help();

        std::unordered_map<std::string, std::vector<std::string>> options();

    private:
        const std::string dipswitch_flag = "-d";
        const std::string gui_flag = "-g";
        const std::string help_flag_short = "-h";
        const std::string help_flag_long = "--help";

        std::vector<std::string> m_args;
        bool m_is_asking_for_help;
        std::string m_short_executable_name;
        std::unordered_map<std::string, std::vector<std::string>> m_options;

        void parse_flag(const std::string &flag, std::vector<std::string> args, std::size_t arg_idx);
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_OPTIONS_H
