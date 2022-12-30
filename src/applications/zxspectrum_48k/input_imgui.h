#pragma once

#include "interfaces/input.h"
#include "key_request.h"
#include <SDL_scancode.h>
#include <functional>
#include <string>
#include <vector>

namespace emu::applications::zxspectrum_48k {
class CpuIo;
class GuiIo;
}
namespace emu::applications::zxspectrum_48k {
class KeyObserver;
}

namespace emu::applications::zxspectrum_48k {

class InputImgui : public Input {
public:
    void read(CpuIo& cpu_io, GuiIo& gui_io) override;

    void read_debug_only(CpuIo& cpu_io, GuiIo& gui_io) override;

    void add_io_observer(KeyObserver& observer) override;

    void remove_io_observer(KeyObserver* observer) override;

private:
    static constexpr SDL_Scancode s_mute = SDL_SCANCODE_SCROLLLOCK;
    static constexpr SDL_Scancode s_pause = SDL_SCANCODE_PAUSE;
    static constexpr SDL_Scancode s_step_instruction = SDL_SCANCODE_F7;
    static constexpr SDL_Scancode s_step_cycle = SDL_SCANCODE_F8;
    static constexpr SDL_Scancode s_continue_running = SDL_SCANCODE_F9;

    static constexpr unsigned int s_SHIFT_bit = 0;
    static constexpr unsigned int s_Z_bit = 1;
    static constexpr unsigned int s_X_bit = 2;
    static constexpr unsigned int s_C_bit = 3;
    static constexpr unsigned int s_V_bit = 4;

    static constexpr unsigned int s_A_bit = 0;
    static constexpr unsigned int s_S_bit = 1;
    static constexpr unsigned int s_D_bit = 2;
    static constexpr unsigned int s_F_bit = 3;
    static constexpr unsigned int s_G_bit = 4;

    static constexpr unsigned int s_Q_bit = 0;
    static constexpr unsigned int s_W_bit = 1;
    static constexpr unsigned int s_E_bit = 2;
    static constexpr unsigned int s_R_bit = 3;
    static constexpr unsigned int s_T_bit = 4;

    static constexpr unsigned int s_1_bit = 0;
    static constexpr unsigned int s_2_bit = 1;
    static constexpr unsigned int s_3_bit = 2;
    static constexpr unsigned int s_4_bit = 3;
    static constexpr unsigned int s_5_bit = 4;

    static constexpr unsigned int s_0_bit = 0;
    static constexpr unsigned int s_9_bit = 1;
    static constexpr unsigned int s_8_bit = 2;
    static constexpr unsigned int s_7_bit = 3;
    static constexpr unsigned int s_6_bit = 4;

    static constexpr unsigned int s_P_bit = 0;
    static constexpr unsigned int s_O_bit = 1;
    static constexpr unsigned int s_I_bit = 2;
    static constexpr unsigned int s_U_bit = 3;
    static constexpr unsigned int s_Y_bit = 4;

    static constexpr unsigned int s_ENTER_bit = 0;
    static constexpr unsigned int s_L_bit = 1;
    static constexpr unsigned int s_K_bit = 2;
    static constexpr unsigned int s_J_bit = 3;
    static constexpr unsigned int s_H_bit = 4;

    static constexpr unsigned int s_SPACE_bit = 0;
    static constexpr unsigned int s_SYM_bit = 1;
    static constexpr unsigned int s_M_bit = 2;
    static constexpr unsigned int s_N_bit = 3;
    static constexpr unsigned int s_B_bit = 4;

    std::vector<KeyObserver*> m_io_observers;

    std::vector<std::function<void()>> m_cancel_last_keypress;

    void notify_io_observers(KeyRequest request);

    void cancel_shift(CpuIo& cpu_io);

    void handle_text(CpuIo& cpu_io, std::string text, bool is_shift_pressed);
};
}
