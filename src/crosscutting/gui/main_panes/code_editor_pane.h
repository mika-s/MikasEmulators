#ifndef MIKA_EMULATORS_CROSSCUTTING_GUI_MAIN_PANES_CODE_EDITOR_PANE_H
#define MIKA_EMULATORS_CROSSCUTTING_GUI_MAIN_PANES_CODE_EDITOR_PANE_H

#include "crosscutting/debugging/debug_container.h"
#include "imgui.h"
#include <iostream>

namespace emu::gui {

    using emu::debugger::DebugContainer;

    class CodeEditorPaneObserver {
    public:
        virtual ~CodeEditorPaneObserver() = default;

        virtual void source_code_changed(const std::string &source_code) = 0;

        virtual void assemble_and_load_request() = 0;
    };

    template<class A, class D>
    class CodeEditorPane {
    public:
        void attach_debug_container(DebugContainer<A, D> &debug_container) {
            m_debug_container = debug_container;
            m_is_debug_container_set = true;
        }

        void draw(const char *title, bool *p_open = nullptr) {
            if (!ImGui::Begin(title, p_open, ImGuiWindowFlags_MenuBar)) {
                ImGui::End();
                return;
            }

            if (!m_is_debug_container_set) {
                ImGui::Text("The debug container is not provided this pane.");
            } else if (!m_debug_container.is_file_content_set()) {
                ImGui::Text("The file content is not provided to this pane.");
            } else {
                ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

                strncpy(buffer, m_debug_container.file_content().c_str(), buffer_size);

                const ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;
                ImGui::InputTextMultiline("##code", buffer, IM_ARRAYSIZE(buffer), //
                                          ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight() - 30.0f), flags);
                if (ImGui::IsItemDeactivatedAfterEdit()) {
                    notify_pane_observers_about_source_code_change();
                }

                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor::HSV(2 / 7.0f, 0.6f, 0.6f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor::HSV(2 / 7.0f, 0.7f, 0.7f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor::HSV(2 / 7.0f, 0.8f, 0.8f));
                if (ImGui::Button("Assemble and load", ImVec2(200.0f, 25.0f))) {
                    notify_pane_observers_about_assemble_and_load_request();
                }
                ImGui::PopStyleColor(3);

                ImGui::EndChild();
            }

            ImGui::End();
        }

        void add_pane_observer(CodeEditorPaneObserver &observer) {
            m_pane_observers.push_back(&observer);
        }

        void remove_pane_observer(CodeEditorPaneObserver *observer) {
            m_pane_observers.erase(
                    std::remove(m_pane_observers.begin(), m_pane_observers.end(), observer),
                    m_pane_observers.end()
            );
        }

    private:
        static constexpr unsigned int buffer_size = 65536;

        DebugContainer<A, D> m_debug_container;
        bool m_is_debug_container_set{false};

        char buffer[buffer_size]; // NOLINT

        std::vector<CodeEditorPaneObserver *> m_pane_observers;

        void notify_pane_observers_about_source_code_change() {
            for (CodeEditorPaneObserver *observer: m_pane_observers) {
                observer->source_code_changed(std::string(buffer));
            }
        }

        void notify_pane_observers_about_assemble_and_load_request() {
            for (CodeEditorPaneObserver *observer: m_pane_observers) {
                observer->assemble_and_load_request();
            }
        }
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_GUI_MAIN_PANES_CODE_EDITOR_PANE_H
