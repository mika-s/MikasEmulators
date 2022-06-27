#ifndef MIKA_EMULATORS_8080_FRONTEND_H
#define MIKA_EMULATORS_8080_FRONTEND_H

#include <vector>
#include <memory>
#include "8080/cpu.h"
#include "8080/applications/space_invaders/space_invaders.h"
#include "8080/applications/space_invaders/settings.h"
#include "8080/interfaces/emulator8080.h"
#include "crosscutting/gui/gui_type.h"

namespace emu::i8080 {

    using emu::gui::GuiType;
    using emu::i8080::applications::space_invaders::Settings;

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
                const Settings &settings,
                GuiType gui_type
        );

        static void disassemble(const std::string &file_path);

        static void test();

        static std::unique_ptr<Emulator8080> choose_emulator(
                const std::string &program,
                const Settings &settings,
                GuiType gui_type
        );

        static Settings find_space_invaders_settings(
                std::unordered_map<std::string, std::vector<std::string>> options
        );

        static GuiType find_gui_type(
                std::unordered_map<std::string, std::vector<std::string>> options
        );
    };
}

#endif //MIKA_EMULATORS_8080_FRONTEND_H
