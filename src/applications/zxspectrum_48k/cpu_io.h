#pragma once

#include "crosscutting/typedefs.h"

namespace emu::applications::zxspectrum_48k {

class CpuIo {
public:
    CpuIo();

    u8 m_in_port0xfe;
    u8 m_out_port0xfe;

private:

};
}
