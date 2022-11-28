#include "lmc_out.h"
#include "assembler/scanner.h"
#include "assembler/token_kind.h"

namespace emu::lmc {
    class InstructionInterface;
}

namespace emu::lmc {

    LmcOut::LmcOut() = default;

    Data LmcOut::eval() {
        return opcode;
    }

    std::unique_ptr<InstructionInterface> LmcOut::parse(Scanner &scanner) {
        scanner.skip(TokenKind::Out);

        return std::make_unique<LmcOut>();
    }
}
