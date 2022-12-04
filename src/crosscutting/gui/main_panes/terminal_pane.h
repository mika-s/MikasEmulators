#ifndef MIKA_EMULATORS_CROSSCUTTING_GUI_MAIN_PANES_TERMINAL_PANE_H
#define MIKA_EMULATORS_CROSSCUTTING_GUI_MAIN_PANES_TERMINAL_PANE_H

namespace emu::gui {

    class TerminalPane {
    public:
        TerminalPane();

        void draw(const char *title, bool *p_open = nullptr);

    private:
        static constexpr unsigned int output_buffer_size = 65536;
        static constexpr unsigned int input_buffer_size = 4;

        char output_buffer[output_buffer_size]; // NOLINT
        char input_buffer[input_buffer_size]; // NOLINT
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_GUI_MAIN_PANES_TERMINAL_PANE_H
