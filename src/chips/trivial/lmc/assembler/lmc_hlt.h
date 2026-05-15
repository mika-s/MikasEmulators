#pragma once

#include "instruction_interface.h"
#include "usings.h"
#include <memory>

namespace emu::lmc {
class Scanner;
}

namespace emu::lmc {

class LmcHlt : public InstructionInterface {
public:
    LmcHlt();

    auto eval() -> Data override;

    static auto parse(Scanner& scanner) -> std::unique_ptr<InstructionInterface>;

private:
    static const inline auto opcode = Data(0);
};
}
