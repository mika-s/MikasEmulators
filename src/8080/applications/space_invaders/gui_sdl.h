#ifndef MIKA_EMULATORS_8080_APPLICATIONS_SPACE_INVADERS_GUI_SDL_H
#define MIKA_EMULATORS_8080_APPLICATIONS_SPACE_INVADERS_GUI_SDL_H

#include <cstdint>
#include <vector>
#include <SDL_video.h>
#include <SDL_render.h>
#include "8080/applications/space_invaders/interfaces/gui.h"

namespace emu::cpu8080::applications::space_invaders {

    class GuiSdl: public Gui {
    public:
        GuiSdl();

        ~GuiSdl() override;

        void update_screen(const std::vector<std::uint8_t> &vram) override;

    private:
        static constexpr float scale = 4.0;
        static constexpr int width = 224;
        static constexpr int height = 256;
        static constexpr int colors = 3;
        static constexpr int bits_in_byte = 8;
        static constexpr int scaled_width = (int) (scale * (float) width);
        static constexpr int scaled_height = (int) (scale * (float) height);

        SDL_Window *win;
        SDL_Renderer *rend;
        SDL_Texture *texture;

        void init();
    };
}

#endif //MIKA_EMULATORS_8080_APPLICATIONS_SPACE_INVADERS_GUI_SDL_H
