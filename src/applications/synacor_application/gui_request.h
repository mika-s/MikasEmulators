#pragma once

#include "chips/trivial/synacor/usings.h"

namespace emu::applications::synacor {

using emu::synacor::Data;

enum GuiRequestType {
    RUN,
    PAUSE,
    STOP,
    DEBUG_MODE,
    INPUT_FROM_TERMINAL,
};

struct GuiRequest {
    GuiRequestType m_type;
    bool m_bool_payload { false };
    Data m_data_payload { Data(0) };
    std::string m_string_payload { "" };
};

}
