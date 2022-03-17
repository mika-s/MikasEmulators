#include <algorithm>
#include <experimental/iterator>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <sstream>
#include "cpm_application.h"
#include "disassembler8080.h"
#include "frontend.h"
#include "file_util.h"
#include "invalid_program_arguments_exception.h"

namespace emu::cpu8080 {

    using emu::util::exceptions::InvalidProgramArgumentsException;
    using emu::util::file::read_file_into_vector;

    void Frontend::run(
            int argc,
            char *argv[],
            std::unordered_map<std::string, std::vector<std::string>> options
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
                        SpaceInvadersSettings settings = find_space_invaders_settings(std::move(options));
                        ordinary(program, settings);
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

    void Frontend::ordinary(const std::string &program, const SpaceInvadersSettings &settings) {
        choose_emulator(program, settings)->run();
    }

    void Frontend::disassemble(const std::string &program) {
        EmulatorMemory memory;
        memory.add(read_file_into_vector(program));

        Disassembler8080 disassembler(memory);
        disassembler.disassemble();
    }

    std::unique_ptr<Emulator8080> Frontend::choose_emulator(
            const std::string &program,
            const SpaceInvadersSettings &settings
    ) {
        if (program == "space_invaders") {
            return std::make_unique<SpaceInvaders>(settings);
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

    SpaceInvadersSettings Frontend::find_space_invaders_settings(
            std::unordered_map<std::string, std::vector<std::string>> options
    ) {
        SpaceInvadersSettings settings{};
        settings.number_of_lives = NumberOfLives::Three;
        settings.bonus_life_at = BonusLifeAt::_1500;
        settings.coin_info = CoinInfo::On;

        for (auto &opt: options["-d"]) {
            switch (opt[0]) {
            case 'n':
                if (opt == "n=3") {
                    settings.number_of_lives = NumberOfLives::Three;
                } else if (opt == "n=4") {
                    settings.number_of_lives = NumberOfLives::Four;
                } else if (opt == "n=5") {
                    settings.number_of_lives = NumberOfLives::Five;
                } else if (opt == "n=6") {
                    settings.number_of_lives = NumberOfLives::Six;
                } else {
                    std::stringstream ss;
                    ss << "Invalid number of lives passed to the -d option: " << opt
                       << R"(. Should be "-d n=3", ..., "-d n=6".)";
                    throw InvalidProgramArgumentsException(ss.str());
                }
                break;
            case 'b':
                if (opt == "b=1500") {
                    settings.bonus_life_at = BonusLifeAt::_1500;
                } else if (opt == "b=1000") {
                    settings.bonus_life_at = BonusLifeAt::_1000;
                } else {
                    std::stringstream ss;
                    ss << "Invalid bonus life at passed to the -d option: " << opt
                       << R"(. Should be "-d b=1500" or "-d b=1000".)";
                    throw InvalidProgramArgumentsException(ss.str());
                }
                break;
            case 'c':
                if (opt == "c=on") {
                    settings.coin_info = CoinInfo::On;
                } else if (opt == "c=off") {
                    settings.coin_info = CoinInfo::Off;
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
}
