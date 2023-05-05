#pragma once

namespace emu::applications::game_boy {

enum GuiRequestType {
    RUN,
    PAUSE,
    STOP,
    DEBUG_MODE,
};

struct GuiRequest {
    GuiRequestType m_type;
    bool m_payload { false };
};

}
