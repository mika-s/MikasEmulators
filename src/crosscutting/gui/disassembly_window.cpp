#include "disassembly_window.h"

namespace emu::util::gui {
    DisassemblyWindow::DisassemblyWindow() {
    }

    void DisassemblyWindow::draw(const char *title, bool *p_open) {
        if (!ImGui::Begin(title, p_open)) {
            ImGui::End();
            return;
        }

        ImGui::Text("Placeholder");

        ImGui::End();
    }
}
