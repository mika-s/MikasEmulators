#include "gui_imgui.h"
#include "chips/trivial/lmc/interfaces/ui_observer.h"
#include "chips/trivial/lmc/out_type.h"
#include "chips/trivial/lmc/usings.h"
#include "glad/glad.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include "lmc_application/lmc_memory_editor.h"
#include "lmc_application/terminal_input_state.h"
#include "ui.h"
#include <SDL.h>
#include <SDL_error.h>
#include <SDL_log.h>
#include <algorithm>
#include <cstdlib>
#include <fmt/core.h>
#include <string>
#include <utility>

namespace emu::debugger {
    template<class A, class D>
    class DebugContainer;
}
namespace emu::debugger {
    class Debugger;
}
namespace emu::logging {
    class Logger;
}

namespace emu::applications::lmc {

    using emu::lmc::Address;
    using emu::lmc::Data;
    using emu::lmc::OutType;
    using emu::misc::RunStatus::NOT_RUNNING;
    using emu::misc::RunStatus::PAUSED;
    using emu::misc::RunStatus::RUNNING;
    using emu::misc::RunStatus::STEPPING;

    GuiImgui::GuiImgui()
        : m_win(nullptr),
          m_gl_context(nullptr),
          m_show_code_editor(true),
          m_show_terminal(true),
          m_show_game_info(true),
          m_show_cpu_info(true),
          m_show_log(true),
          m_show_disassembly(true),
          m_show_memory_editor(true),
          m_is_in_debug_mode(false) {
        init();

        m_code_editor.add_pane_observer(*this);
        m_terminal.add_pane_observer(*this);
    }

    GuiImgui::~GuiImgui() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();

        SDL_GL_DeleteContext(m_gl_context);
        SDL_DestroyWindow(m_win);
        SDL_QuitSubSystem(SDL_INIT_VIDEO);

        m_win = nullptr;
        m_gl_context = nullptr;
    }

    void GuiImgui::to_terminal(Data acc_reg, OutType out_type) {
        if (out_type == OutType::OUT) {
            m_output.push_back(fmt::format("{}\n", acc_reg.underlying()));
        } else {
            m_output.push_back(fmt::format("{}", static_cast<char>(acc_reg.underlying())));
        }
    }

    void GuiImgui::from_terminal() {
    }

    void GuiImgui::clear_terminal() {
        m_output.clear();
    }

    void GuiImgui::add_ui_observer(UiObserver &observer) {
        m_gui_observers.push_back(&observer);
    }

    void GuiImgui::remove_ui_observer(UiObserver *observer) {
        m_gui_observers.erase(
                std::remove(m_gui_observers.begin(), m_gui_observers.end(), observer),
                m_gui_observers.end()
        );
    }

    void GuiImgui::notify_ui_observers_about_run_status(RunStatus new_status) {
        for (UiObserver *observer: m_gui_observers) {
            observer->run_status_changed(new_status);
        }
    }

    void GuiImgui::notify_ui_observers_about_debug_mode() {
        for (UiObserver *observer: m_gui_observers) {
            observer->debug_mode_changed(m_is_in_debug_mode);
        }
    }

    void GuiImgui::notify_ui_observers_about_source_code_change(const std::string &source_code) {
        for (UiObserver *observer: m_gui_observers) {
            observer->source_code_changed(source_code);
        }
    }

    void GuiImgui::notify_ui_observers_about_assemble_and_load_request() {
        for (UiObserver *observer: m_gui_observers) {
            observer->assemble_and_load_request();
        }
    }

    void GuiImgui::notify_ui_observers_about_input_from_terminal(Data input) {
        for (UiObserver *observer: m_gui_observers) {
            observer->input_from_terminal(input);
        }
    }

    void GuiImgui::attach_debugger(std::shared_ptr<Debugger> debugger) {
        m_disassembly.attach_debugger(debugger);
    }

    void GuiImgui::attach_debug_container(DebugContainer<Address, Data> &debug_container) {
        m_cpu_info.attach_debug_container(debug_container);
        //        m_disassembly.attach_debug_container(debug_container);
        m_memory_editor.attach_debug_container(debug_container);
        m_code_editor.attach_debug_container(debug_container);
    }

    void GuiImgui::attach_logger(std::shared_ptr<Logger> logger) {
        m_logger = std::move(logger);
        m_log.attach_logger(m_logger);
        m_disassembly.attach_logger(m_logger);
    }

    void GuiImgui::init() {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "error initializing SDL video: %s", SDL_GetError());
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
                "Little Man Computer",
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                scaled_width,
                scaled_height,
                SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_MAXIMIZED
        );
        if (!m_win) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "error creating SDL window: %s", SDL_GetError());
            exit(1);
        }

        SDL_SetWindowMinimumSize(m_win, width, height);
        m_gl_context = SDL_GL_CreateContext(m_win);
        SDL_GL_MakeCurrent(m_win, m_gl_context);
        SDL_GL_SetSwapInterval(1);

        if (!gladLoadGLLoader(static_cast<GLADloadproc>(SDL_GL_GetProcAddress))) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "error initializing glad");
            exit(1);
        }

        glViewport(0, 0, width, height);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void) io;
        io.IniFilename = "lmc_imgui.ini";
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        ImGui::StyleColorsDark();

        ImGui_ImplSDL2_InitForOpenGL(m_win, m_gl_context);
        ImGui_ImplOpenGL3_Init(glsl_version.c_str());

        const ImVec4 background = ImVec4(35 / 255.0f, 35 / 255.0f, 35 / 255.0f, 1.00f);
        glClearColor(background.x, background.y, background.z, background.w);
    }

    void GuiImgui::update_screen(RunStatus run_status, TerminalInputState terminal_input_state) {
        render(run_status, terminal_input_state);
    }

    void GuiImgui::render(RunStatus run_status, TerminalInputState terminal_input_state) {
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
        ImGui::SetNextWindowSize(ImVec2(static_cast<float>(window_width), static_cast<float>(window_height)), ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(0.0f);

        ImGui::Begin("Main window", nullptr,
                     ImGuiWindowFlags_NoResize                        //
                             | ImGuiWindowFlags_NoCollapse            //
                             | ImGuiWindowFlags_NoMove                //
                             | ImGuiWindowFlags_NoTitleBar            //
                             | ImGuiWindowFlags_NoScrollbar           //
                             | ImGuiWindowFlags_NoScrollWithMouse     //
                             | ImGuiWindowFlags_NoBringToFrontOnFocus //
                             | ImGuiWindowFlags_NoNavFocus            //
                             | ImGuiWindowFlags_NoFocusOnAppearing    //
                             | ImGuiWindowFlags_NoBackground          //
                             | ImGuiWindowFlags_NoDocking             //
                             | ImGuiWindowFlags_MenuBar);
        ImGui::PopStyleVar();

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Exit", "Alt+F4")) {
                    notify_ui_observers_about_run_status(NOT_RUNNING);
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Windows")) {
                ImGui::MenuItem("Code editor", nullptr, &m_show_code_editor);
                ImGui::MenuItem("Terminal", nullptr, &m_show_terminal);
                ImGui::MenuItem("Program info", nullptr, &m_show_game_info);
                ImGui::MenuItem("CPU info", nullptr, &m_show_cpu_info);
                ImGui::MenuItem("Log", nullptr, &m_show_log);
                ImGui::MenuItem("Disassembly", nullptr, &m_show_disassembly);
                ImGui::MenuItem("Memory editor", nullptr, &m_show_memory_editor);
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
        if (m_show_code_editor) {
            render_code_editor();
        }
        if (m_show_terminal) {
            render_terminal_window(run_status, terminal_input_state);
        }
        if (m_show_game_info) {
            render_game_info_window();
        }
        if (m_show_cpu_info) {
            render_cpu_info_window();
        }
        if (m_show_memory_editor) {
            render_memory_editor_window();
        }

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(m_win);
    }

    void GuiImgui::update_debug_only(TerminalInputState terminal_input_state) {
        render(STEPPING, terminal_input_state);
    }

    void GuiImgui::render_code_editor() {
        m_code_editor.draw("Code editor", &m_show_code_editor);
    }

    void GuiImgui::render_terminal_window(RunStatus run_status, TerminalInputState terminal_input_state) {
        std::string prefix = "Program";
        std::string id = "###" + prefix;
        std::string terminal_status = "";
        if (terminal_input_state == AWAITING_INPUT) {
            terminal_status = " (awaiting input)";
        }
        std::string title;
        if (run_status == RUNNING) {
            title = prefix + terminal_status + id;
        } else if (run_status == PAUSED) {
            title = prefix + " - Paused" + terminal_status + id;
        } else if (run_status == NOT_RUNNING) {
            title = prefix + " - Stopped" + terminal_status + id;
        } else if (run_status == STEPPING) {
            title = prefix + " - Stepping" + terminal_status + id;
        } else {
            title = "Unknown TODO " + terminal_status + id;
        }


        m_terminal.draw(title.c_str(), terminal_input_state == AWAITING_INPUT, m_output, &m_show_terminal);
    }

    void GuiImgui::render_game_info_window() {
        ImGui::Begin("Program info", &m_show_game_info);

        ImGui::Text("Avg %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        ImGui::Separator();

        if (ImGui::Button("Run")) {
            notify_ui_observers_about_run_status(RUNNING);
        }
        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
        if (ImGui::Button("Pause")) {
            notify_ui_observers_about_run_status(PAUSED);
        }
        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
        if (ImGui::Button("Stop")) {
            notify_ui_observers_about_run_status(NOT_RUNNING);
        }
        ImGui::Separator();
        if (ImGui::Checkbox("Debug mode", &m_is_in_debug_mode)) {
            notify_ui_observers_about_debug_mode();
        }

        ImGui::End();
    }

    void GuiImgui::render_cpu_info_window() {
        m_cpu_info.draw("CPU info", &m_show_cpu_info);
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

    void GuiImgui::source_code_changed(const std::string &source_code) {
        notify_ui_observers_about_source_code_change(source_code);
    }

    void GuiImgui::assemble_and_load_request() {
        notify_ui_observers_about_assemble_and_load_request();
    }

    void GuiImgui::input_sent(const std::string &input) {
        notify_ui_observers_about_input_from_terminal(Data(std::stoi(input)));
    }
}
