#include "lmc_inp.h"
#include "assembler/scanner.h"
#include "assembler/token_kind.h"
#include <memory>

namespace emu::lmc {
class InstructionInterface;
}

namespace emu::lmc {

LmcInp::LmcInp() = default;

auto LmcInp::eval() -> Data
{
    return opcode;
}

auto LmcInp::parse(Scanner& scanner) -> std::unique_ptr<InstructionInterface>
{
    scanner.skip(TokenKind::Inp);

    return std::make_unique<LmcInp>();
}
}
