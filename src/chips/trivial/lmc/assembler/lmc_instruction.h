#pragma once

#include "instruction_interface.h"
#include "usings.h"
#include <memory>

namespace emu::lmc {
class Scanner;
}
namespace emu::lmc {
class Environment;
}

namespace emu::lmc {
class LmcInstruction {
public:
    explicit LmcInstruction(std::unique_ptr<InstructionInterface> instruction);

    [[nodiscard]] auto eval() const -> Data;

    static auto parse(Scanner& scanner, Environment& environment) -> std::unique_ptr<LmcInstruction>;

private:
    std::unique_ptr<InstructionInterface> m_instruction;
};
}
