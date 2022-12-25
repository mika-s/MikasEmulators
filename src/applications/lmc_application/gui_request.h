#pragma once

#include "chips/trivial/lmc/usings.h"

namespace emu::applications::lmc {

using emu::lmc::Data;

enum GuiRequestType {
    RUN,
    PAUSE,
    STOP,
    DEBUG_MODE,
    SOURCE_CODE,
    ASSEMBLE_AND_LOAD,
    INPUT_FROM_TERMINAL,
};

struct GuiRequest {
    GuiRequestType m_type;
    bool m_bool_payload { false };
    Data m_data_payload { Data(0) };
    std::string m_string_payload { "" };
};

}
