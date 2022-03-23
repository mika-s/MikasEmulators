#ifndef MIKA_EMULATORS_8080_APPLICATIONS_SPACE_INVADERS_GUI_IMGUI_H
#define MIKA_EMULATORS_8080_APPLICATIONS_SPACE_INVADERS_GUI_IMGUI_H

#include <cstdint>
#include <vector>
#include <SDL_video.h>
#include <SDL_render.h>
#include "8080/applications/space_invaders/interfaces/gui.h"
#include "crosscutting/gui/debug_log.h"

namespace emu::cpu8080::applications::space_invaders {

    class GuiImgui : public Gui {
    public:
        GuiImgui();

        ~GuiImgui() override;

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
        SDL_GLContext gl_context;

        std::uint32_t screen_texture;
        std::uint32_t screen_pixels[width * height];

        bool show_game;
        bool show_game_info;
        bool show_log;
        bool show_demo;

        emu::util::gui::DebugLog log;

        void init();

        void render();

        void render_game_window();

        void render_game_info_window();

        void render_log_window();
    };
}

#endif //MIKA_EMULATORS_8080_APPLICATIONS_SPACE_INVADERS_GUI_IMGUI_H
