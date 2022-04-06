#include <algorithm>
#include <experimental/iterator>
#include <iostream>
#include <unordered_map>
#include <sstream>
#include "frontend.h"
#include "8080/disassembler8080.h"
#include "8080/applications/cpm/cpm_application.h"
#include "8080/applications/space_invaders/space_invaders.h"
#include "8080/applications/space_invaders/input_imgui.h"
#include "8080/applications/space_invaders/input_sdl.h"
#include "8080/applications/space_invaders/gui_imgui.h"
#include "8080/applications/space_invaders/gui_sdl.h"
#include "crosscutting/file_util.h"
#include "crosscutting/invalid_program_arguments_exception.h"

namespace emu::cpu8080 {

    using emu::cpu8080::applications::space_invaders::Settings;
    using emu::util::exceptions::InvalidProgramArgumentsException;
    using emu::util::gui::GuiType;

    void Frontend::run(
            int argc, char *argv[],
            const std::unordered_map<std::string, std::vector<std::string>>& options
    ) {
        if (argc > 2) {
            const std::string mode(argv[2]);

            if (mode == "disassemble") {
                if (argc == 4) {
                    const std::string file_path(argv[3]);
                    disassemble(file_path);
                } else {
                    throw InvalidProgramArgumentsException("Wrong number of arguments to disassemble mode.");
                }
            } else if (mode == "run") {
                if (argc >= 4) {
                    const std::string program(argv[3]);

                    if (is_supporting(program)) {
                        const Settings settings = find_space_invaders_settings(options);
                        const GuiType gui_type = find_gui_type(options);
                        ordinary(program, settings, gui_type);
                    } else {
                        throw InvalidProgramArgumentsException("Unsupported game or program.");
                    }
                } else {
                    throw InvalidProgramArgumentsException("Wrong number of arguments to run mode.");
                }
            } else {
                std::stringstream ss;
                ss << "Unknown mode: " << mode << "\n";
                throw InvalidProgramArgumentsException(ss.str());
            }
        } else {
            throw InvalidProgramArgumentsException("Wrong number of arguments to 8080.");
        }
    }

    bool Frontend::is_supporting(const std::string &program) {
        return std::find(supported_programs.begin(), supported_programs.end(), program) != supported_programs.end();
    }

    std::string Frontend::supported() {
        const char *const delim = ", ";

        std::ostringstream imploded;
        std::copy(supported_programs.begin(), supported_programs.end(),
                  std::experimental::ostream_joiner<std::string>(imploded, delim));

        return imploded.str();
    }

    void Frontend::ordinary(const std::string &program, const Settings &settings, GuiType gui_type) {
        choose_emulator(program, settings, gui_type)->run();
    }

    void Frontend::disassemble(const std::string &program) {
        using emu::util::file::read_file_into_vector;

        EmulatorMemory memory;
        memory.add(read_file_into_vector(program));

        Disassembler8080 disassembler(memory, std::cout);
        disassembler.disassemble();
    }

    std::unique_ptr<Emulator8080> Frontend::choose_emulator(
            const std::string &program,
            const Settings &settings,
            GuiType gui_type
    ) {
        using emu::cpu8080::applications::cpm::CpmApplication;
        using emu::cpu8080::applications::space_invaders::InputSdl;
        using emu::cpu8080::applications::space_invaders::InputImgui;
        using emu::cpu8080::applications::space_invaders::GuiSdl;
        using emu::cpu8080::applications::space_invaders::GuiImgui;
        using emu::cpu8080::applications::space_invaders::SpaceInvaders;

        if (program == "space_invaders") {
            if (gui_type == GuiType::DEBUGGING) {
                return std::make_unique<SpaceInvaders>(
                        settings,
                        std::make_shared<GuiImgui>(),
                        std::make_shared<InputImgui>()
                );
            } else {
                return std::make_unique<SpaceInvaders>(
                        settings,
                        std::make_shared<GuiSdl>(),
                        std::make_shared<InputSdl>()
                );
            }
        } else if (program == "TST8080") {
            return std::make_unique<CpmApplication>("TST8080.COM");
        } else if (program == "8080PRE") {
            return std::make_unique<CpmApplication>("8080PRE.COM");
        } else if (program == "8080EXM") {
            return std::make_unique<CpmApplication>("8080EXM.COM");
        } else if (program == "CPUTEST") {
            return std::make_unique<CpmApplication>("CPUTEST.COM");
        } else {
            throw std::invalid_argument("Illegal program argument when choosing emulator");
        }
    }

    Settings Frontend::find_space_invaders_settings(
            std::unordered_map<std::string, std::vector<std::string>> options
    ) {
        using emu::cpu8080::applications::space_invaders::BonusLifeAt;
        using emu::cpu8080::applications::space_invaders::CoinInfo;
        using emu::cpu8080::applications::space_invaders::NumberOfLives;

        Settings settings{};
        settings.m_number_of_lives = NumberOfLives::Three;
        settings.m_bonus_life_at = BonusLifeAt::_1500;
        settings.m_coin_info = CoinInfo::On;

        for (auto &opt: options["-d"]) {
            switch (opt[0]) {
                case 'n':
                    if (opt == "n=3") {
                        settings.m_number_of_lives = NumberOfLives::Three;
                    } else if (opt == "n=4") {
                        settings.m_number_of_lives = NumberOfLives::Four;
                    } else if (opt == "n=5") {
                        settings.m_number_of_lives = NumberOfLives::Five;
                    } else if (opt == "n=6") {
                        settings.m_number_of_lives = NumberOfLives::Six;
                    } else {
                        std::stringstream ss;
                        ss << "Invalid number of lives passed to the -d option: " << opt
                           << R"(. Should be "-d n=3", ..., "-d n=6".)";
                        throw InvalidProgramArgumentsException(ss.str());
                    }
                    break;
                case 'b':
                    if (opt == "b=1500") {
                        settings.m_bonus_life_at = BonusLifeAt::_1500;
                    } else if (opt == "b=1000") {
                        settings.m_bonus_life_at = BonusLifeAt::_1000;
                    } else {
                        std::stringstream ss;
                        ss << "Invalid bonus life at passed to the -d option: " << opt
                           << R"(. Should be "-d b=1500" or "-d b=1000".)";
                        throw InvalidProgramArgumentsException(ss.str());
                    }
                    break;
                case 'c':
                    if (opt == "c=on") {
                        settings.m_coin_info = CoinInfo::On;
                    } else if (opt == "c=off") {
                        settings.m_coin_info = CoinInfo::Off;
                    } else {
                        std::stringstream ss;
                        ss << "Invalid coin info passed to the -d option: " << opt
                           << R"(. Should be "-d c=on" or "-d c=off".)";
                        throw InvalidProgramArgumentsException(ss.str());
                    }
                    break;
                default:
                    std::stringstream ss;
                    ss << "Unknown -d value: " << opt << "\n";
                    throw InvalidProgramArgumentsException(ss.str());
            }
        }

        return settings;
    }

    emu::util::gui::GuiType Frontend::find_gui_type(
            std::unordered_map<std::string, std::vector<std::string>> options
    ) {
        if (!options.contains("-g")) {
            return GuiType::ORDINARY;
        }
        if (options["-g"].size() > 1) {
            throw InvalidProgramArgumentsException("-g flag should only be used once");
        } else if (options["-g"].empty()) {
            throw InvalidProgramArgumentsException("-g flag needs an additional argument");
        }

        if (options["-g"][0] == "ordinary") {
            return GuiType::ORDINARY;
        } else if (options["-g"][0] == "debugging") {
            return GuiType::DEBUGGING;
        } else {
            throw InvalidProgramArgumentsException("Unknown GUI type passed to the -g flag");
        }
    }
}
