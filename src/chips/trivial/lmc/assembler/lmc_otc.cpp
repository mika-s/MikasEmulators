#include "lmc_otc.h"
#include "assembler/scanner.h"
#include "assembler/token_kind.h"

namespace emu::lmc {
class InstructionInterface;
}

namespace emu::lmc {

LmcOtc::LmcOtc() = default;

auto LmcOtc::eval() -> Data
{
    return opcode;
}

auto LmcOtc::parse(Scanner& scanner) -> std::unique_ptr<InstructionInterface>
{
    scanner.skip(TokenKind::Otc);

    return std::make_unique<LmcOtc>();
}
}
