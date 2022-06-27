#include <algorithm>
#include <experimental/iterator>
#include <iostream>
#include <unordered_map>
#include <sstream>
#include "doctest.h"
#include "frontend.h"
#include "z80/disassemblerZ80.h"
#include "z80/applications/cpm/cpm_application.h"
#include "crosscutting/exceptions/invalid_program_arguments_exception.h"
#include "crosscutting/util/file_util.h"

namespace emu::z80 {

    using emu::exceptions::InvalidProgramArgumentsException;

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
            [[maybe_unused]] const Settings &settings,
            [[maybe_unused]] GuiType gui_type
    ) {
        using applications::cpm::CpmApplication;

        if (program == "pacman") {
            throw InvalidProgramArgumentsException("pacman not implemented yet");
        } else if (program == "prelim") {
            return std::make_unique<CpmApplication>("prelim.z80");
        } else if (program == "zexall") {
            return std::make_unique<CpmApplication>("zexall.z80");
        } else if (program == "zexdoc") {
            return std::make_unique<CpmApplication>("zexdoc.z80");
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
        using applications::pacman::BonusLifeAt;
        using applications::pacman::CoinInfo;
        using applications::pacman::NumberOfLives;

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
