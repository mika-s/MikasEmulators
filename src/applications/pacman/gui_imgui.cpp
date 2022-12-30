#include "gui_imgui.h"
#include "crosscutting/gui/graphics/framebuffer.h"
#include "crosscutting/logging/logger.h"
#include "glad/glad.h"
#include "gui_request.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include "interfaces/gui_observer.h"
#include "pacman/gui.h"
#include <SDL.h>
#include <SDL_error.h>
#include <SDL_log.h>
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <unordered_map>
#include <utility>

namespace emu::debugger {
template<class A, class D, std::size_t B>
class DebugContainer;
}
namespace emu::debugger {
template<class A, std::size_t B>
class Debugger;
}

namespace emu::applications::pacman {

GuiImgui::GuiImgui()
{
    init();
}

GuiImgui::~GuiImgui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(m_gl_context);
    SDL_DestroyWindow(m_win);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);

    m_win = nullptr;
    m_gl_context = nullptr;
}

void GuiImgui::add_gui_observer(GuiObserver& observer)
{
    m_gui_observers.push_back(&observer);
}

void GuiImgui::remove_gui_observer(GuiObserver* observer)
{
    m_gui_observers.erase(
        std::remove(m_gui_observers.begin(), m_gui_observers.end(), observer),
        m_gui_observers.end());
}

void GuiImgui::notify_gui_observers(GuiRequest request)
{
    for (GuiObserver* observer : m_gui_observers) {
        observer->gui_request(request);
    }
}

void GuiImgui::attach_debugger(std::shared_ptr<Debugger<u16, 16>> debugger)
{
    m_disassembly.attach_debugger(debugger);
}

void GuiImgui::attach_debug_container(std::shared_ptr<DebugContainer<u16, u8, 16>> debug_container)
{
    m_cpu_info.attach_debug_container(debug_container);
    m_io_info.attach_debug_container(debug_container);
    m_disassembly.attach_debug_container(debug_container);
    m_memory_editor.attach_debug_container(debug_container);
    m_tilemap.attach_debug_container(debug_container);
    m_spritemap.attach_debug_container(debug_container);
    m_waveforms.attach_debug_container(debug_container);
}

void GuiImgui::attach_logger(std::shared_ptr<Logger> logger)
{
    m_logger = std::move(logger);
    m_log.attach_logger(m_logger);
    m_disassembly.attach_logger(m_logger);
}

void GuiImgui::toggle_tile_debug()
{
    m_is_tile_debug_enabled = !m_is_tile_debug_enabled;
    m_logger->info(m_is_tile_debug_enabled ? "Tile debug: on" : "Tile debug: off");
    m_framebuffer.clear();
}

void GuiImgui::toggle_sprite_debug()
{
    m_is_sprite_debug_enabled = !m_is_sprite_debug_enabled;
    m_logger->info(m_is_sprite_debug_enabled ? "Sprite debug: on" : "Sprite debug: off");
    m_framebuffer.clear();
}

void GuiImgui::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "error initializing SDL: %s", SDL_GetError());
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
        SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
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
        "Pacman",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        s_scaled_width,
        s_scaled_height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_MAXIMIZED);
    if (!m_win) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "error creating SDL window: %s", SDL_GetError());
        exit(1);
    }

    SDL_SetWindowMinimumSize(m_win, s_width, s_height);
    m_gl_context = SDL_GL_CreateContext(m_win);
    if (!m_gl_context) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "error creating context: %s", SDL_GetError());
        exit(1);
    }
    SDL_GL_MakeCurrent(m_win, m_gl_context);
    SDL_GL_SetSwapInterval(1);

    if (!gladLoadGLLoader(static_cast<GLADloadproc>(SDL_GL_GetProcAddress))) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "error initializing glad");
        exit(1);
    }

    glViewport(0, 0, s_width, s_height);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.IniFilename = "pacman_imgui.ini";
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(m_win, m_gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version.c_str());

    const ImVec4 background = ImVec4(35 / 255.0f, 35 / 255.0f, 35 / 255.0f, 1.00f);
    glClearColor(background.x, background.y, background.z, background.w);

    glGenTextures(1, &m_screen_texture);
    glGenTextures(1, &m_tile_texture);
}

void GuiImgui::update_screen(
    std::vector<u8> const& tile_ram,
    std::vector<u8> const& sprite_ram,
    std::vector<u8> const& palette_ram,
    bool is_screen_flipped,
    std::string const& game_window_subtitle)
{
    std::vector<u32> framebuffer = create_framebuffer(tile_ram, sprite_ram, palette_ram, is_screen_flipped);

    glBindTexture(GL_TEXTURE_2D, m_screen_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, s_width, s_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, framebuffer.data());
    glBindTexture(GL_TEXTURE_2D, 0);

    render(game_window_subtitle);
}

void GuiImgui::render(std::string const& game_window_subtitle)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(m_win);
    ImGui::NewFrame();

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0.0f;

    int window_width;
    int window_height;
    SDL_GetWindowSize(m_win, &window_width, &window_height);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowPos(ImVec2(.0f, .0f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(static_cast<float>(window_width), static_cast<float>(window_height)), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.0f);

    ImGui::Begin("Main window", nullptr,
        ImGuiWindowFlags_NoResize                    //
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
                notify_gui_observers({ .m_type = GuiRequestType::STOP });
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
            ImGui::MenuItem("Tilemap", nullptr, &m_show_tilemap);
            ImGui::MenuItem("Spritemap", nullptr, &m_show_spritemap);
            ImGui::MenuItem("Waveforms", nullptr, &m_show_waveforms);
            ImGui::MenuItem("Demo", nullptr, &m_show_demo);
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::DockSpace(
        ImGui::GetID("Docking"),
        ImVec2(0.0f, 0.0f),
        ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode);

    if (m_show_log) {
        render_log_pane();
    }
    if (m_show_disassembly) {
        render_disassembly_pane();
    }
    if (m_show_game) {
        render_game_pane(game_window_subtitle);
    }
    if (m_show_game_info) {
        render_game_info_pane();
    }
    if (m_show_cpu_info) {
        render_cpu_info_pane();
    }
    if (m_show_io_info) {
        render_io_info_pane();
    }
    if (m_show_memory_editor) {
        render_memory_editor_pane();
    }
    if (m_show_tilemap) {
        render_tilemap_pane();
    }
    if (m_show_spritemap) {
        render_spritemap_pane();
    }
    if (m_show_waveforms) {
        render_waveform_pane();
    }
    if (m_show_demo) {
        ImGui::ShowDemoWindow();
    }

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    SDL_GL_SwapWindow(m_win);
}

void GuiImgui::update_debug_only()
{
    render("Stepping");
}

void GuiImgui::render_game_pane(std::string const& game_window_subtitle)
{
    const std::string prefix = "Game";
    const std::string id = "###" + prefix;
    const std::string title = game_window_subtitle.empty() ? prefix + id : prefix + " - " + game_window_subtitle + id;

    ImGui::Begin(title.c_str(), &m_show_game);

    const ImVec2 image_size = ImVec2(s_scaled_width, s_scaled_height);
    ImGui::Image(
        (void*)((intptr_t)m_screen_texture), image_size,
        ImVec2(0, 0),
        ImVec2(1, 1),
        ImColor(255, 255, 255, 255),
        ImColor(0, 0, 0, 0));

    ImGui::End();
}

void GuiImgui::render_game_info_pane()
{
    ImGui::Begin("Game info", &m_show_game_info);

    ImGui::Text("Avg %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::Separator();

    if (ImGui::Button("Run")) {
        notify_gui_observers({ .m_type = GuiRequestType::RUN });
    }
    ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
    if (ImGui::Button("Pause")) {
        notify_gui_observers({ .m_type = GuiRequestType::PAUSE });
    }
    ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
    if (ImGui::Button("Stop")) {
        notify_gui_observers({ .m_type = GuiRequestType::STOP });
    }
    ImGui::Separator();
    if (ImGui::Checkbox("Debug mode", &m_is_in_debug_mode)) {
        notify_gui_observers({ .m_type = GuiRequestType::DEBUG_MODE, .m_payload = m_is_in_debug_mode });
    }

    ImGui::End();
}

void GuiImgui::render_cpu_info_pane()
{
    m_cpu_info.draw("CPU info", &m_show_cpu_info);
}

void GuiImgui::render_io_info_pane()
{
    m_io_info.draw("IO info", &m_show_io_info);
}

void GuiImgui::render_log_pane()
{
    m_log.draw("Log", &m_show_log);
}

void GuiImgui::render_disassembly_pane()
{
    m_disassembly.draw("Disassembly", &m_show_disassembly);
}

void GuiImgui::render_memory_editor_pane()
{
    m_memory_editor.draw("Memory editor", &m_show_memory_editor);
}

void GuiImgui::render_tilemap_pane()
{
    m_tilemap.draw("Tilemap", m_tile_texture, &m_show_tilemap);
}

void GuiImgui::render_spritemap_pane()
{
    m_spritemap.draw("Spritemap", m_sprite_texture, &m_show_spritemap);
}

void GuiImgui::render_waveform_pane()
{
    m_waveforms.draw("Waveforms", &m_show_waveforms);
}
}
