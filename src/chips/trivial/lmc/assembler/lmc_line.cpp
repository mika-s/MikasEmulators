#include "lmc_line.h"

#include "assembler/lmc_instruction.h"
#include "assembler/lmc_label.h"
#include "assembler/scanner.h"
#include "assembler/token_kind.h"
#include <memory>
#include <optional>
#include <utility>

namespace emu::lmc {
    class Environment;
}

namespace emu::lmc {

    LmcLine::LmcLine(std::optional<LmcLabel> label, std::unique_ptr<LmcInstruction> instruction)
        : m_label(std::move(label)),
          m_instruction(std::move(instruction)) {
    }

    Data LmcLine::eval() {
        return m_instruction->eval();
    }

    LmcLine LmcLine::parse(Scanner &scanner, Environment &environment) {
        std::optional<LmcLabel> label = LmcLabel::parse(scanner, environment);
        std::unique_ptr<LmcInstruction> instruction = LmcInstruction::parse(scanner, environment);

        scanner.skip(TokenKind::Newline);

        return {label, std::move(instruction)};
    }
}
