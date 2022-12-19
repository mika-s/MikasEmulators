#pragma once

namespace emu::applications::zxspectrum_48k {

class GuiIo {
public:
    bool m_is_toggling_pause { false };
    bool m_is_quitting { false };
    bool m_is_stepping_cycle { false };
    bool m_is_stepping_instruction { false };
    bool m_is_continuing_execution { false };
};

}
