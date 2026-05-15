#include "lmc_hlt.h"
#include "assembler/scanner.h"
#include "assembler/token_kind.h"
#include <memory>

namespace emu::lmc {
class InstructionInterface;
}

namespace emu::lmc {

LmcHlt::LmcHlt() = default;

auto LmcHlt::eval() -> Data
{
    return opcode;
}

auto LmcHlt::parse(Scanner& scanner) -> std::unique_ptr<InstructionInterface>
{
    scanner.skip(TokenKind::Hlt);

    return std::make_unique<LmcHlt>();
}
}
