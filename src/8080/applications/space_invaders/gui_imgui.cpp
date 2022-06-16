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
            : m_win(nullptr),
              m_gl_context(nullptr),
              m_screen_texture(0),
              m_show_game(true),
              m_show_game_info(true),
              m_show_cpu_info(true),
              m_show_io_info(true),
              m_show_log(true),
              m_show_disassembly(true),
              m_show_memory_editor(true),
              m_show_demo(false),
              m_is_in_debug_mode(false) {
        init();
    }

    GuiImgui::~GuiImgui() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();

        SDL_GL_DeleteContext(m_gl_context);
        SDL_DestroyWindow(m_win);
        SDL_Quit();

        m_win = nullptr;
        m_gl_context = nullptr;
    }

    void GuiImgui::add_gui_observer(GuiObserver &observer) {
        m_gui_observers.push_back(&observer);
    }

    void GuiImgui::remove_gui_observer(GuiObserver *observer) {
        m_gui_observers.erase(
                std::remove(m_gui_observers.begin(), m_gui_observers.end(), observer),
                m_gui_observers.end()
        );
    }

    void GuiImgui::notify_gui_observers_about_run_status(RunStatus new_status) {
        for (GuiObserver *observer: m_gui_observers) {
            observer->run_status_changed(new_status);
        }
    }

    void GuiImgui::notify_gui_observers_about_debug_mode() {
        for (GuiObserver *observer: m_gui_observers) {
            observer->debug_mode_changed(m_is_in_debug_mode);
        }
    }

    void GuiImgui::attach_debugger(std::shared_ptr<Debugger> debugger) {
        m_disassembly.attach_debugger(debugger);
    }

    void GuiImgui::attach_debug_container(DebugContainer &debug_container) {
        m_cpu_info.attach_debug_container(debug_container);
        m_io_info.attach_debug_container(debug_container);
        m_disassembly.attach_debug_container(debug_container);
        m_memory_editor.attach_debug_container(debug_container);
    }

    void GuiImgui::attach_logger(std::shared_ptr<Logger> logger) {
        m_logger = std::move(logger);
        m_log.attach_logger(m_logger);
        m_disassembly.attach_logger(m_logger);
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

        m_win = SDL_CreateWindow(
                "Space Invaders",
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                scaled_width,
                scaled_height,
                SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_MAXIMIZED
        );
        if (!m_win) {
            std::cerr << "error creating SDL window: " << SDL_GetError() << "\n";
            exit(1);
        }

        SDL_SetWindowMinimumSize(m_win, width, height);
        m_gl_context = SDL_GL_CreateContext(m_win);
        SDL_GL_MakeCurrent(m_win, m_gl_context);
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

        ImGui_ImplSDL2_InitForOpenGL(m_win, m_gl_context);
        ImGui_ImplOpenGL3_Init(glsl_version.c_str());

        const ImVec4 background = ImVec4(35 / 255.0f, 35 / 255.0f, 35 / 255.0f, 1.00f);
        glClearColor(background.x, background.y, background.z, background.w);

        glGenTextures(1, &m_screen_texture);
    }

    void GuiImgui::update_screen(const std::vector<std::uint8_t> &vram, RunStatus run_status) {
        std::vector<std::uint32_t> frame_buffer = create_framebuffer(vram);

        glBindTexture(GL_TEXTURE_2D, m_screen_texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, frame_buffer.data());

        glBindTexture(GL_TEXTURE_2D, 0);

        render(run_status);
    }

    void GuiImgui::render(RunStatus run_status) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(m_win);
        ImGui::NewFrame();

        ImGuiStyle &style = ImGui::GetStyle();
        style.WindowRounding = 0.0f;

        int window_width;
        int window_height;
        SDL_GetWindowSize(m_win, &window_width, &window_height);

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
                    notify_gui_observers_about_run_status(NOT_RUNNING);
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Windows")) {
                ImGui::MenuItem("Game", nullptr, &m_show_game);
                ImGui::MenuItem("Game info", nullptr, &m_show_game_info);
                ImGui::MenuItem("CPU info", nullptr, &m_show_cpu_info);
                ImGui::MenuItem("IO info", nullptr, &m_show_io_info);
                ImGui::MenuItem("Log", nullptr, &m_show_log);
                ImGui::MenuItem("Disassembly", nullptr, &m_show_disassembly);
                ImGui::MenuItem("Memory editor", nullptr, &m_show_memory_editor);
                ImGui::MenuItem("Demo", nullptr, &m_show_demo);
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::DockSpace(
                ImGui::GetID("Docking"),
                ImVec2(0.0f, 0.0f),
                ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode
        );

        if (m_show_log) {
            render_log_window();
        }
        if (m_show_disassembly) {
            render_disassembly_window();
        }
        if (m_show_game) {
            render_game_window(run_status);
        }
        if (m_show_game_info) {
            render_game_info_window();
        }
        if (m_show_cpu_info) {
            render_cpu_info_window();
        }
        if (m_show_io_info) {
            render_io_info_window();
        }
        if (m_show_memory_editor) {
            render_memory_editor_window();
        }
        if (m_show_demo) {
            ImGui::ShowDemoWindow();
        }

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(m_win);
    }

    void GuiImgui::update_debug_only() {
        render(STEPPING);
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
            title = prefix + " - Stopped" + id;
        } else if (run_status == STEPPING) {
            title = prefix + " - Stepping" + id;
        } else {
            title = "Unknown TODO" + id;
        }

        ImGui::Begin(title.c_str(), &m_show_game);

        const ImVec2 image_size = ImVec2(scaled_width, scaled_height);
        ImGui::Image(
                (void *) ((intptr_t) m_screen_texture), image_size,
                ImVec2(0, 0),
                ImVec2(1, 1),
                ImColor(255, 255, 255, 255),
                ImColor(0, 0, 0, 0)
        );

        ImGui::End();
    }

    void GuiImgui::render_game_info_window() {
        ImGui::Begin("Game info", &m_show_game);

        ImGui::Text("Avg %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        ImGui::Separator();

        if (ImGui::Button("Run")) {
            notify_gui_observers_about_run_status(RUNNING);
        }
        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
        if (ImGui::Button("Pause")) {
            notify_gui_observers_about_run_status(PAUSED);
        }
        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
        if (ImGui::Button("Stop")) {
            notify_gui_observers_about_run_status(NOT_RUNNING);
        }
        ImGui::Separator();
        if (ImGui::Checkbox("Debug mode", &m_is_in_debug_mode)) {
            notify_gui_observers_about_debug_mode();
        }

        ImGui::End();
    }

    void GuiImgui::render_cpu_info_window() {
        m_cpu_info.draw("CPU info", &m_show_cpu_info);
    }

    void GuiImgui::render_io_info_window() {
        m_io_info.draw("IO info", &m_show_io_info);
    }

    void GuiImgui::render_log_window() {
        m_log.draw("Log", &m_show_log);
    }

    void GuiImgui::render_disassembly_window() {
        m_disassembly.draw("Disassembly", &m_show_disassembly);
    }

    void GuiImgui::render_memory_editor_window() {
        m_memory_editor.draw("Memory editor", &m_show_memory_editor);
    }
}
