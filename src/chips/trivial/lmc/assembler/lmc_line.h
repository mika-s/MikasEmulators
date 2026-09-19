#pragma once

#include "lmc_instruction.h"
#include "lmc_label.h"
#include "usings.h"
#include <memory>
#include <optional>

namespace emu::lmc {
class Scanner;
}
namespace emu::lmc {
class Environment;
}

namespace emu::lmc {

class LmcLine {

public:
    LmcLine(std::optional<LmcLabel> label, std::unique_ptr<LmcInstruction> instruction);

    [[nodiscard]] auto eval() const -> Data;

    static auto parse(Scanner& scanner, Environment& environment) -> LmcLine;

private:
    std::optional<LmcLabel> m_label;
    std::unique_ptr<LmcInstruction> m_instruction;
};
}
