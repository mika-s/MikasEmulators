#include "lmc_otc.h"
#include "assembler/scanner.h"
#include "assembler/token_kind.h"

namespace emu::lmc {
    class InstructionInterface;
}

namespace emu::lmc {

    LmcOtc::LmcOtc() = default;

    Data LmcOtc::eval() {
        return opcode;
    }

    std::unique_ptr<InstructionInterface> LmcOtc::parse(Scanner &scanner) {
        scanner.skip(TokenKind::Otc);

        return std::make_unique<LmcOtc>();
    }
}
