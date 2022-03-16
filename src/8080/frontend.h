#ifndef MIKA_EMULATORS_8080_FRONTEND_H
#define MIKA_EMULATORS_8080_FRONTEND_H

#include <vector>
#include "cpu.h"
#include "space_invaders.h"

namespace emu::cpu8080 {
    class Frontend {
    public:
        static void run(
                int argc,
                char *argv[],
                std::unordered_map<std::string, std::vector<std::string>> options
        );

        static bool is_supporting(const std::string& program);

        static std::string supported();

    private:
        static const inline std::vector<std::string> supported_programs = {
                "space_invaders",
                "TST8080",
                "8080PRE",
                "8080EXM",
                "CPUTEST"
        };

        static void ordinary(const std::string &program, const SpaceInvadersSettings &settings);

        static void disassemble(const std::string &file_path);

        static std::unique_ptr<Emulator8080> choose_emulator(
                const std::string &program,
                const SpaceInvadersSettings &settings
        );

        static SpaceInvadersSettings find_space_invaders_settings(
                std::unordered_map<std::string, std::vector<std::string>> options
        );
    };
}

#endif //MIKA_EMULATORS_8080_FRONTEND_H
