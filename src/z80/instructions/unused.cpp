#include <iostream>
#include "crosscutting/misc/next_word.h"
#include "crosscutting/util/string_util.h"

namespace emu::z80 {

    using emu::misc::NextWord;
    using emu::util::string::hexify;
    using emu::util::string::hexify_wo_0x;

    void print_unused_jmp(std::ostream& ostream, const NextWord &args) {
        ostream << "*JMP "
                << hexify_wo_0x(args.sarg)
                << hexify_wo_0x(args.farg);
    }

    void print_unused_ret(std::ostream& ostream) {
        ostream << "*RET";
    }
}
