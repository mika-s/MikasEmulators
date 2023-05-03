#include "lmc_instruction.h"
#include "assembler/instruction_interface.h"
#include "assembler/scanner.h"
#include "assembler/token.h"
#include "assembler/token_kind.h"
#include "lmc_add.h"
#include "lmc_bra.h"
#include "lmc_brp.h"
#include "lmc_brz.h"
#include "lmc_dat.h"
#include "lmc_hlt.h"
#include "lmc_inp.h"
#include "lmc_lda.h"
#include "lmc_otc.h"
#include "lmc_out.h"
#include "lmc_sta.h"
#include "lmc_sub.h"
#include <fmt/core.h>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

namespace emu::lmc {

LmcInstruction::LmcInstruction(std::unique_ptr<InstructionInterface> instruction)
    : m_instruction(std::move(instruction))
{
}

Data LmcInstruction::eval()
{
    return m_instruction->eval();
}

std::unique_ptr<LmcInstruction> LmcInstruction::parse(Scanner& scanner, Environment& environment)
{
    const Token current_token = scanner.current_token();

    switch (current_token.kind()) {
    case TokenKind::Add:
        return std::make_unique<LmcInstruction>(LmcAdd::parse(scanner, environment));
    case TokenKind::Sub:
        return std::make_unique<LmcInstruction>(LmcSub::parse(scanner, environment));
    case TokenKind::Sta:
        return std::make_unique<LmcInstruction>(LmcSta::parse(scanner, environment));
    case TokenKind::Lda:
        return std::make_unique<LmcInstruction>(LmcLda::parse(scanner, environment));
    case TokenKind::Bra:
        return std::make_unique<LmcInstruction>(LmcBra::parse(scanner, environment));
    case TokenKind::Brz:
        return std::make_unique<LmcInstruction>(LmcBrz::parse(scanner, environment));
    case TokenKind::Brp:
        return std::make_unique<LmcInstruction>(LmcBrp::parse(scanner, environment));
    case TokenKind::Inp:
        return std::make_unique<LmcInstruction>(LmcInp::parse(scanner));
    case TokenKind::Out:
        return std::make_unique<LmcInstruction>(LmcOut::parse(scanner));
    case TokenKind::Otc:
        return std::make_unique<LmcInstruction>(LmcOtc::parse(scanner));
    case TokenKind::Cob:
    case TokenKind::Hlt:
        return std::make_unique<LmcInstruction>(LmcHlt::parse(scanner));
    case TokenKind::Dat:
        return std::make_unique<LmcInstruction>(LmcDat::parse(scanner));
    default:
        throw std::invalid_argument(
            fmt::format(
                "Invalid token when parsing instruction. Received {}.",
                TokenKind_as_string.at(current_token.kind())));
    }
}
}
