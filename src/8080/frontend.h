#ifndef MIKA_EMULATORS_8080_FRONTEND_H
#define MIKA_EMULATORS_8080_FRONTEND_H

#include <vector>
#include "8080/cpu.h"
#include "8080/applications/space_invaders/space_invaders.h"
#include "8080/applications/space_invaders/settings.h"
#include "8080/interfaces/emulator8080.h"
#include "crosscutting/gui/gui_type.h"

namespace emu::cpu8080 {
    class Frontend {
    public:
        static void run(
                int argc,
                char *argv[],
                const std::unordered_map<std::string, std::vector<std::string>>& options
        );

        static bool is_supporting(const std::string &program);

        static std::string supported();

    private:
        static const inline std::vector<std::string> supported_programs = {
                "space_invaders",
                "TST8080",
                "8080PRE",
                "8080EXM",
                "CPUTEST"
        };

        static void ordinary(
                const std::string &program,
                const emu::cpu8080::applications::space_invaders::Settings &settings,
                emu::util::gui::GuiType gui_type
        );

        static void disassemble(const std::string &file_path);

        static std::unique_ptr<Emulator8080> choose_emulator(
                const std::string &program,
                const emu::cpu8080::applications::space_invaders::Settings &settings,
                emu::util::gui::GuiType gui_type
        );

        static emu::cpu8080::applications::space_invaders::Settings find_space_invaders_settings(
                std::unordered_map<std::string, std::vector<std::string>> options
        );

        static emu::util::gui::GuiType find_gui_type(
                std::unordered_map<std::string, std::vector<std::string>> options
        );
    };
}

#endif //MIKA_EMULATORS_8080_FRONTEND_H
