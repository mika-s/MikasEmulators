#ifndef MIKA_EMULATORS_Z80_FRONTEND_H
#define MIKA_EMULATORS_Z80_FRONTEND_H

#include <vector>
#include <memory>
#include "z80/cpu.h"
//#include "z80/applications/space_invaders/space_invaders.h"
#include "z80/applications/pacman/settings.h"
#include "z80/interfaces/emulatorZ80.h"
#include "crosscutting/gui/gui_type.h"

namespace emu::z80 {

    using emu::gui::GuiType;
    using emu::z80::applications::pacman::Settings;

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
                "pacman",
                "prelim",
                "zexall",
                "zexdoc"
        };

        static void ordinary(
                const std::string &program,
                const Settings &settings,
                GuiType gui_type
        );

        static void disassemble(const std::string &file_path);

        static void test();

        static std::unique_ptr<EmulatorZ80> choose_emulator(
                const std::string &program,
                const Settings &settings,
                GuiType gui_type
        );

        static Settings find_pacman_settings(
                std::unordered_map<std::string, std::vector<std::string>> options
        );

        static GuiType find_gui_type(
                std::unordered_map<std::string, std::vector<std::string>> options
        );
    };
}

#endif //MIKA_EMULATORS_Z80_FRONTEND_H
