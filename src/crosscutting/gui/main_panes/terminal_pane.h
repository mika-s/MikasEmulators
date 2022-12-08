#pragma once

#include <string>
#include <vector>

namespace emu::gui {

class TerminalPaneObserver {
public:
    virtual ~TerminalPaneObserver() = default;

    virtual void input_sent(std::string const& input) = 0;
};

class TerminalPane {
public:
    TerminalPane();

    void add_pane_observer(TerminalPaneObserver& observer);

    void remove_pane_observer(TerminalPaneObserver* observer);

    void draw(
        char const* title,
        bool is_awaiting_input,
        std::vector<std::string> const& output,
        bool* p_open = nullptr);

private:
    static constexpr unsigned int output_buffer_size = 65536;
    static constexpr unsigned int input_buffer_size = 4;

    char m_output_buffer[output_buffer_size]; // NOLINT
    char m_input_buffer[input_buffer_size];   // NOLINT

    std::vector<TerminalPaneObserver*> m_pane_observers;

    void notify_pane_observers_about_new_input();

    void vector_to_output_array(std::vector<std::string> const& output);
};
}
