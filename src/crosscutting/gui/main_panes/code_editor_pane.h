#ifndef MIKA_EMULATORS_CROSSCUTTING_GUI_MAIN_PANES_CODE_EDITOR_PANE_H
#define MIKA_EMULATORS_CROSSCUTTING_GUI_MAIN_PANES_CODE_EDITOR_PANE_H

namespace emu::gui {

    class CodeEditorPane {
    public:
        CodeEditorPane();

        void draw(const char *title, bool *p_open = nullptr);

    private:
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_GUI_MAIN_PANES_CODE_EDITOR_PANE_H
