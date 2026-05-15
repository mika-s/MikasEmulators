#include "lmc_out.h"
#include "assembler/scanner.h"
#include "assembler/token_kind.h"

namespace emu::lmc {
class InstructionInterface;
}

namespace emu::lmc {

LmcOut::LmcOut() = default;

auto LmcOut::eval() -> Data
{
    return opcode;
}

auto LmcOut::parse(Scanner& scanner) -> std::unique_ptr<InstructionInterface>
{
    scanner.skip(TokenKind::Out);

    return std::make_unique<LmcOut>();
}
}
