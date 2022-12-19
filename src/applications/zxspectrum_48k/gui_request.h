#pragma once

namespace emu::applications::zxspectrum_48k {

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
