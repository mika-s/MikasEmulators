#pragma once

#include "instruction_interface.h"
#include "usings.h"
#include <memory>

namespace emu::lmc {
class Scanner;
}

namespace emu::lmc {

class LmcOtc : public InstructionInterface {
public:
    LmcOtc();

    Data eval() override;

    static std::unique_ptr<InstructionInterface> parse(Scanner& scanner);

private:
    static const inline Data opcode = Data(922);
};
}
