#include "lmc_hlt.h"
#include "assembler/scanner.h"
#include "assembler/token_kind.h"
#include <memory>

namespace emu::lmc {
    class InstructionInterface;
}

namespace emu::lmc {

    LmcHlt::LmcHlt() = default;

    Data LmcHlt::eval() {
        return opcode;
    }

    std::unique_ptr<InstructionInterface> LmcHlt::parse(Scanner &scanner) {
        scanner.skip(TokenKind::Hlt);

        return std::make_unique<LmcHlt>();
    }
}
