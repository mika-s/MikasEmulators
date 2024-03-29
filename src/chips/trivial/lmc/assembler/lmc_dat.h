#pragma once

#include "instruction_interface.h"
#include "lmc_integer.h"
#include "usings.h"
#include <memory>
#include <optional>

namespace emu::lmc {
class Scanner;
}

namespace emu::lmc {

class LmcDat : public InstructionInterface {
public:
    LmcDat(std::optional<LmcInteger> operand);

    Data eval() override;

    static std::unique_ptr<InstructionInterface> parse(Scanner& scanner);

private:
    std::optional<LmcInteger> m_operand;
};
}
