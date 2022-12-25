#include "applications/lmc_application/lmc_application.h"
#include "crosscutting/exceptions/rom_file_not_found_exception.h"
#include "crosscutting/gui/gui_type.h"
#include "lmc_application.h"
#include "usage.h"
#include <SDL.h>
#include <iostream>
#include <memory>

using emu::applications::lmc::LmcApplication;
using emu::exceptions::RomFileNotFoundException;
using emu::gui::GuiType;

// NOLINTNEXTLINE(modernize-avoid-c-arrays)
int main(int argc, char* argv[])
{
    try {
        std::make_unique<LmcApplication>("initial.lmc", GuiType::DEBUGGING)
            ->new_session()
            ->run();

        SDL_Quit();
    } catch (RomFileNotFoundException& ex) {
        std::cout << ex.what() << "\n";
        SDL_Quit();
        return 1;
    } catch (std::exception& ex) {
        std::cout << "Generic exception thrown\n";
        std::cout << ex.what() << "\n";
        SDL_Quit();
        return 2;
    }

    return 0;
}
