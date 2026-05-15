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
    explicit LmcDat(std::optional<LmcInteger> operand);

    auto eval() -> Data override;

    static auto parse(Scanner& scanner) -> std::unique_ptr<InstructionInterface>;

private:
    std::optional<LmcInteger> m_operand;
};
}
