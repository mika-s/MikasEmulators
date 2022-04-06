#include "disassembly_window.h"

namespace emu::util::gui {
    DisassemblyWindow::DisassemblyWindow() {
    }

    void DisassemblyWindow::draw(const char *title, bool *p_open) {
        if (!ImGui::Begin(title, p_open)) {
            ImGui::End();
            return;
        }

        // - Scroll when PC leaves the visible area, but not otherwise.

        ImGui::Text("Placeholder");

        ImGui::End();
    }
}
