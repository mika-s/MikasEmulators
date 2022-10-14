#include <algorithm>
#include <experimental/iterator>
#include <iostream>
#include <unordered_map>
#include <sstream>
#include "doctest.h"
#include "frontend.h"
#include "z80/disassemblerZ80.h"
#include "z80/applications/cpm/cpm_application.h"
#include "z80/applications/pacman/pacman.h"
#include "z80/applications/pacman/gui_imgui.h"
#include "z80/applications/pacman/gui_sdl.h"
#include "z80/applications/pacman/input_imgui.h"
#include "z80/applications/pacman/input_sdl.h"
#include "crosscutting/exceptions/invalid_program_arguments_exception.h"
#include "crosscutting/util/file_util.h"

namespace emu::z80 {

    using emu::exceptions::InvalidProgramArgumentsException;

    void Frontend::run(
            std::vector<std::string> args,
            const std::unordered_map<std::string, std::vector<std::string>> &options
    ) {
        if (args.size() > 2) {
            const std::string mode = args[2];

            if (mode == "disassemble") {
                if (args.size() == 4) {
                    const std::string file_path = args[3];
                    disassemble(file_path);
                } else {
                    throw InvalidProgramArgumentsException("Wrong number of arguments to disassemble mode.");
                }
            } else if (mode == "run") {
                if (args.size() >= 4) {
                    const std::string program = args[3];

                    if (is_supporting(program)) {
                        const Settings settings = find_pacman_settings(options);
                        const GuiType gui_type = find_gui_type(options);
                        ordinary(program, settings, gui_type);
                    } else {
                        throw InvalidProgramArgumentsException("Unsupported game or program.");
                    }
                } else {
                    throw InvalidProgramArgumentsException("Wrong number of arguments to run mode.");
                }
            } else if (mode == "test") {
                test();
            } else {
                std::stringstream ss;
                ss << "Unknown mode: " << mode << "\n";
                throw InvalidProgramArgumentsException(ss.str());
            }
        } else {
            throw InvalidProgramArgumentsException("Wrong number of arguments to Z80.");
        }
    }

    bool Frontend::is_supporting(const std::string &program) {
        return std::find(supported_programs.begin(), supported_programs.end(), program) != supported_programs.end();
    }

    std::string Frontend::supported() {
        const char *const delimiter = ", ";

        std::ostringstream imploded;
        std::copy(supported_programs.begin(), supported_programs.end(),
                  std::experimental::ostream_joiner<std::string>(imploded, delimiter));

        return imploded.str();
    }

    void Frontend::ordinary(const std::string &program, const Settings &settings, GuiType gui_type) {
        choose_emulator(program, settings, gui_type)->new_session()->run();
    }

    void Frontend::disassemble(const std::string &program) {
        using emu::util::file::read_file_into_vector;

        EmulatorMemory memory;
        memory.add(read_file_into_vector(program));

        DisassemblerZ80 disassembler(memory, std::cout);
        disassembler.disassemble();
    }

    std::unique_ptr<EmulatorZ80> Frontend::choose_emulator(
            const std::string &program,
            const Settings &settings,
            GuiType gui_type
    ) {
        using applications::cpm::CpmApplication;
        using applications::pacman::InputSdl;
        using applications::pacman::InputImgui;
        using applications::pacman::GuiSdl;
        using applications::pacman::GuiImgui;
        using applications::pacman::Pacman;


        if (program == "pacman") {
            if (gui_type == GuiType::DEBUGGING) {
                return std::make_unique<Pacman>(
                        settings,
                        std::make_shared<GuiImgui>(),
                        std::make_shared<InputImgui>()
                );
            } else {
                return std::make_unique<Pacman>(
                        settings,
                        std::make_shared<GuiSdl>(),
                        std::make_shared<InputSdl>()
                );
            }
        } else if (program == "prelim") {
            return std::make_unique<CpmApplication>("roms/z80/prelim.com");
        } else if (program == "zexall") {
            return std::make_unique<CpmApplication>("roms/z80/zexall.cim");
        } else if (program == "zexdoc") {
            return std::make_unique<CpmApplication>("roms/z80/zexdoc.cim");
        } else {
            throw InvalidProgramArgumentsException("Illegal program argument when choosing emulator");
        }
    }

    void Frontend::test() {
        doctest::Context context;
        context.addFilter("test-case", "Z80*");
        context.addFilter("test-case", "crosscutting*");
        int res = context.run();

        if (context.shouldExit()) {
            exit(res);
        }

        exit(res);
    }

    Settings Frontend::find_pacman_settings(
            std::unordered_map<std::string, std::vector<std::string>> options
    ) {
        using applications::pacman::NumberOfLives;
        using applications::pacman::BonusLifeAt;
        using applications::pacman::CoinsPerGame;
        using applications::pacman::Difficulty;
        using applications::pacman::GhostNames;
        using applications::pacman::BoardTest;

        Settings settings{};
        settings.m_number_of_lives = NumberOfLives::Three;
        settings.m_bonus_life_at = BonusLifeAt::_15000;
        settings.m_coins_per_game = CoinsPerGame::OnePerGame;
        settings.m_difficulty = Difficulty::Normal;
        settings.m_ghost_names = GhostNames::Normal;
        settings.m_board_test = BoardTest::Off;

        for (auto &opt: options["-d"]) {
            switch (opt[0]) {
                case 'n':
                    if (opt == "n=1") {
                        settings.m_number_of_lives = NumberOfLives::One;
                    } else if (opt == "n=2") {
                        settings.m_number_of_lives = NumberOfLives::Two;
                    } else if (opt == "n=3") {
                        settings.m_number_of_lives = NumberOfLives::Three;
                    } else if (opt == "n=5") {
                        settings.m_number_of_lives = NumberOfLives::Five;
                    } else {
                        std::stringstream ss;
                        ss << "Invalid number of lives passed to the -d option: " << opt
                           << R"(. Should be "-d n=1", "-d n=2", "-d n=3" or "-d n=5".)";
                        throw InvalidProgramArgumentsException(ss.str());
                    }
                    break;
                case 'b':
                    if (opt == "b=10000") {
                        settings.m_bonus_life_at = BonusLifeAt::_10000;
                    } else if (opt == "b=15000") {
                        settings.m_bonus_life_at = BonusLifeAt::_15000;
                    } else if (opt == "b=20000") {
                        settings.m_bonus_life_at = BonusLifeAt::_20000;
                    } else if (opt == "b=none") {
                        settings.m_bonus_life_at = BonusLifeAt::None;
                    } else {
                        std::stringstream ss;
                        ss << "Invalid bonus life at passed to the -d option: " << opt
                           << R"(. Should be "-d b=10000", "-d b=15000", "-d b=20000" or "-d b=none".)";
                        throw InvalidProgramArgumentsException(ss.str());
                    }
                    break;
                case 'c':
                    if (opt == "c=opg") {
                        settings.m_coins_per_game = CoinsPerGame::OnePerGame;
                    } else if (opt == "c=optg") {
                        settings.m_coins_per_game = CoinsPerGame::OnePerTwoGames;
                    } else if (opt == "c=tpg") {
                        settings.m_coins_per_game = CoinsPerGame::TwoCoinsPerGame;
                    } else if (opt == "c=free") {
                        settings.m_coins_per_game = CoinsPerGame::FreePlay;
                    } else {
                        std::stringstream ss;
                        ss << "Invalid number of coins per game passed to the -d option: " << opt
                           << R"(. Should be "-d c=opg" (one per game), "-d c=optg" (one per two games), )"
                           << R"("-d c=tpg" (two per game) or "-d c=free".)";
                        throw InvalidProgramArgumentsException(ss.str());
                    }
                    break;
                case 'd':
                    if (opt == "d=normal") {
                        settings.m_difficulty = Difficulty::Normal;
                    } else if (opt == "d=hard") {
                        settings.m_difficulty = Difficulty::Hard;
                    } else {
                        std::stringstream ss;
                        ss << "Invalid difficulty passed to the -d option: " << opt
                           << R"(. Should be "-d d=normal" or "-d d=hard".)";
                        throw InvalidProgramArgumentsException(ss.str());
                    }
                    break;
                case 'g':
                    if (opt == "g=normal") {
                        settings.m_ghost_names = GhostNames::Normal;
                    } else if (opt == "g=alternate") {
                        settings.m_ghost_names = GhostNames::Alternate;
                    } else {
                        std::stringstream ss;
                        ss << "Invalid ghost names passed to the -d option: " << opt
                           << R"(. Should be "-d g=normal" or "-d g=alternate".)";
                        throw InvalidProgramArgumentsException(ss.str());
                    }
                    break;
                case 't':
                    if (opt == "t=off") {
                        settings.m_board_test = BoardTest::Off;
                    } else if (opt == "t=on") {
                        settings.m_board_test = BoardTest::On;
                    } else {
                        std::stringstream ss;
                        ss << "Invalid board test passed to the -d option: " << opt
                           << R"(. Should be "-d t=off" or "-d t=on".)";
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

    GuiType Frontend::find_gui_type(
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
