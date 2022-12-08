#include "lmc_inp.h"
#include "assembler/scanner.h"
#include "assembler/token_kind.h"
#include <memory>

namespace emu::lmc {
class InstructionInterface;
}

namespace emu::lmc {

LmcInp::LmcInp() = default;

Data LmcInp::eval()
{
    return opcode;
}

std::unique_ptr<InstructionInterface> LmcInp::parse(Scanner& scanner)
{
    scanner.skip(TokenKind::Inp);

    return std::make_unique<LmcInp>();
}
}
