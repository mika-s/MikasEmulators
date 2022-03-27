#include <iostream>
#include <SDL.h>
#include <SDL_timer.h>
#include "glad/glad.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "gui_imgui.h"
#include "8080/cpu.h"
#include "crosscutting/byte_util.h"
#include "crosscutting/file_util.h"

namespace emu::cpu8080::applications::space_invaders {

    using emu::util::byte::is_bit_set;

    GuiImgui::GuiImgui()
            : win(nullptr),
              gl_context(nullptr),
              screen_texture(0),
              screen_pixels(),
              show_game(true),
              show_game_info(true),
              show_cpu_info(true),
              show_log(true),
              show_demo(false) {
        init();
    }

    GuiImgui::~GuiImgui() {
//        for (auto observer : gui_observers) {
//            remove_gui_observer(observer);
//        }

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();

        SDL_GL_DeleteContext(gl_context);
        SDL_DestroyWindow(win);
        SDL_Quit();

        win = nullptr;
        gl_context = nullptr;
    }

    void GuiImgui::add_gui_observer(GuiObserver &observer) {
        gui_observers.push_back(&observer);
    }

    void GuiImgui::remove_gui_observer(GuiObserver *observer) {
        gui_observers.erase(
                std::remove(gui_observers.begin(), gui_observers.end(), observer),
                gui_observers.end()
        );
    }

    void GuiImgui::notify_gui_observers(RunStatus new_status) {
        for (GuiObserver *observer: gui_observers) {
            observer->run_status_changed(new_status);
        }
    }

    void GuiImgui::attach_debug_container(DebugContainer &debug_container) {
        cpu_info.attach_debug_container(debug_container);
    }

    void GuiImgui::init() {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
            std::cerr << "error initializing SDL: " << SDL_GetError() << "\n";
            exit(1);
        }

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        std::string glsl_version;
#ifdef __APPLE__
        // GL 3.2 Core + GLSL 150
        glsl_version = "#version 150";
        SDL_GL_SetAttribute( // required on Mac OS
            SDL_GL_CONTEXT_FLAGS,
            SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG
            );
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
#elif __linux__
        // GL 3.2 Core + GLSL 150
        glsl_version = "#version 150";
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#elif _WIN32
        // GL 3.0 + GLSL 130
        glsl_version = "#version 130";
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#endif

        win = SDL_CreateWindow(
                "Space Invaders",
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                scaled_width,
                scaled_height,
                SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_MAXIMIZED
        );
        if (!win) {
            std::cerr << "error creating SDL window: " << SDL_GetError() << "\n";
            exit(1);
        }

        SDL_SetWindowMinimumSize(win, width, height);
        gl_context = SDL_GL_CreateContext(win);
        SDL_GL_MakeCurrent(win, gl_context);
        SDL_GL_SetSwapInterval(1);

        if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
            std::cerr << "error initializing glad\n";
            exit(1);
        }

        glViewport(0, 0, width, height);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void) io;
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        ImGui::StyleColorsDark();

        ImGui_ImplSDL2_InitForOpenGL(win, gl_context);
        ImGui_ImplOpenGL3_Init(glsl_version.c_str());

        const ImVec4 background = ImVec4(35 / 255.0f, 35 / 255.0f, 35 / 255.0f, 1.00f);
        glClearColor(background.x, background.y, background.z, background.w);

        glGenTextures(1, &screen_texture);
        memset(screen_pixels, 0, sizeof(screen_pixels));
    }

    void GuiImgui::update_screen(const std::vector<std::uint8_t> &vram, RunStatus run_status) {
        std::uint8_t screen[height][width][colors];

        for (int i = 0; i < height * width / bits_in_byte; i++) {
            const int y = i * bits_in_byte / height;
            const int base_x = (i * bits_in_byte) % height;
            const std::uint8_t current_byte = vram[i];

            for (std::uint8_t bit = 0; bit < bits_in_byte; bit++) {
                int px = base_x + bit;
                int py = y;
                const bool is_pixel_lit = is_bit_set(current_byte, bit);
                std::uint8_t r = 0;
                std::uint8_t g = 0;
                std::uint8_t b = 0;

                if (is_pixel_lit) {
                    if (px < 16) {
                        if (py < 16 || 134 < py) {
                            r = 255;
                            g = 255;
                            b = 255;
                        } else {
                            g = 255;
                        }
                    } else if (16 <= px && px <= 72) {
                        g = 255;
                    } else if (192 <= px && px < 224) {
                        r = 255;
                    } else {
                        r = 255;
                        g = 255;
                        b = 255;
                    }
                }

                const int temp_x = px;
                px = py;
                py = -temp_x + height - 1;

                screen[py][px][0] = r;
                screen[py][px][1] = g;
                screen[py][px][2] = b;
            }
        }

        int pixel_idx = 0;
        for (auto &height_idx: screen) {
            for (auto &width_idx: height_idx) {
                std::uint8_t r = width_idx[0];
                std::uint8_t g = width_idx[1];
                std::uint8_t b = width_idx[2];
                screen_pixels[pixel_idx++] = 0xFF000000 | b << 16 | g << 8 | r;
            }
        }

        glBindTexture(GL_TEXTURE_2D, screen_texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, screen_pixels);

        glBindTexture(GL_TEXTURE_2D, 0);

        render(run_status);
    }

    void GuiImgui::render(RunStatus run_status) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(win);
        ImGui::NewFrame();

        ImGuiStyle &style = ImGui::GetStyle();
        style.WindowRounding = 0.0f;

        int window_width;
        int window_height;
        SDL_GetWindowSize(win, &window_width, &window_height);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::SetNextWindowPos(ImVec2(.0f, .0f), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(static_cast<float>(window_width), static_cast<float>(window_height)),
                                 ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(0.0f);

        ImGui::Begin("Main window", nullptr,
                     ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoTitleBar |
                     ImGuiWindowFlags_NoScrollbar |
                     ImGuiWindowFlags_NoScrollWithMouse |
                     ImGuiWindowFlags_NoBringToFrontOnFocus |
                     ImGuiWindowFlags_NoNavFocus |
                     ImGuiWindowFlags_NoFocusOnAppearing |
                     ImGuiWindowFlags_NoBackground |
                     ImGuiWindowFlags_NoDocking |
                     ImGuiWindowFlags_MenuBar
        );
        ImGui::PopStyleVar();

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Exit", "Alt+F4")) {
                    notify_gui_observers(NOT_RUNNING);
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Windows")) {
                ImGui::MenuItem("Game", nullptr, &show_game);
                ImGui::MenuItem("Game info", nullptr, &show_game_info);
                ImGui::MenuItem("CPU info", nullptr, &show_cpu_info);
                ImGui::MenuItem("Log", nullptr, &show_log);
                ImGui::MenuItem("Demo", nullptr, &show_demo);
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::DockSpace(
                ImGui::GetID("Docking"),
                ImVec2(0.0f, 0.0f),
                ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode
        );

        if (show_log) {
            render_log_window();
        }
        if (show_game) {
            render_game_window(run_status);
        }
        if (show_game_info) {
            render_game_info_window();
        }
        if (show_cpu_info) {
            render_cpu_info_window();
        }
        if (show_demo) {
            ImGui::ShowDemoWindow();
        }

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(win);
    }

    void GuiImgui::render_game_window(RunStatus run_status) {
        std::string prefix = "Game";
        std::string id = "###" + prefix;
        std::string title;
        if (run_status == RUNNING) {
            title = prefix + id;
        } else if (run_status == PAUSED) {
            title = prefix + " - Paused" + id;
        } else if (run_status == NOT_RUNNING) {
            title = prefix + "- Stopped" + id;
        }

        ImGui::Begin(title.c_str(), &show_game);

        const ImVec2 image_size = ImVec2(scaled_width, scaled_height);
        ImGui::Image(
                (void *) ((intptr_t) screen_texture), image_size,
                ImVec2(0, 0),
                ImVec2(1, 1),
                ImColor(255, 255, 255, 255),
                ImColor(0, 0, 0, 0)
        );

        ImGui::End();
    }

    void GuiImgui::render_game_info_window() {
        ImGui::Begin("Game info", &show_game);

        ImGui::Text("Avg %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        ImGui::Separator();

        if (ImGui::Button("Run")) {
            notify_gui_observers(RUNNING);
        }
        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
        if (ImGui::Button("Pause")) {
            notify_gui_observers(PAUSED);
        }
        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
        if (ImGui::Button("Stop")) {
//            notify_gui_observers(NOT_RUNNING);
        }

        ImGui::End();
    }

    void GuiImgui::render_cpu_info_window() {
        cpu_info.draw("CPU info", &show_cpu_info);
    }

    void GuiImgui::render_log_window() {
        log.draw("Log", &show_log);
    }
}
