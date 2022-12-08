#pragma once

#include <string>
#include <unordered_map>

namespace emu::lmc {
enum class TokenKind {
    Label,
    Integer,
    Newline,
    Eof,
    Add,
    Sub,
    Sta,
    Lda,
    Bra,
    Brz,
    Brp,
    Inp,
    Out,
    Otc,
    Hlt,
    Cob,
    Dat
};

static const inline std::unordered_map<TokenKind, std::string> TokenKind_as_string = {
    { TokenKind::Label, "Label" },
    { TokenKind::Integer, "Integer" },
    { TokenKind::Newline, "Newline" },
    { TokenKind::Eof, "Eof" },
    { TokenKind::Add, "Add" },
    { TokenKind::Sub, "Sub" },
    { TokenKind::Sta, "Sta" },
    { TokenKind::Lda, "Lda" },
    { TokenKind::Bra, "Bra" },
    { TokenKind::Brz, "Brz" },
    { TokenKind::Brp, "Brp" },
    { TokenKind::Inp, "Inp" },
    { TokenKind::Out, "Out" },
    { TokenKind::Otc, "Otc" },
    { TokenKind::Hlt, "Hlt" },
    { TokenKind::Cob, "Cob" },
    { TokenKind::Dat, "Dat" },
};

static const inline std::unordered_map<std::string, TokenKind> keyword_string_as_TokenKind = {
    { "ADD", TokenKind::Add },
    { "SUB", TokenKind::Sub },
    { "STA", TokenKind::Sta },
    { "LDA", TokenKind::Lda },
    { "BRA", TokenKind::Bra },
    { "BRZ", TokenKind::Brz },
    { "BRP", TokenKind::Brp },
    { "INP", TokenKind::Inp },
    { "OUT", TokenKind::Out },
    { "OTC", TokenKind::Otc },
    { "HLT", TokenKind::Hlt },
    { "COB", TokenKind::Cob },
    { "DAT", TokenKind::Dat },
};
}
