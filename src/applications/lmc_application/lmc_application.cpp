#include "lmc_application.h"
#include "chips/trivial/lmc/assembler.h"
#include "chips/trivial/lmc/usings.h"
#include "crosscutting/util/byte_util.h"
#include "crosscutting/util/file_util.h"
#include "crosscutting/util/string_util.h"
#include "lmc_application_session.h"
#include <iosfwd>
#include <vector>

namespace emu::misc {
    class Session;
}

namespace emu::applications::lmc {

    using emu::lmc::Assembler;
    using emu::lmc::Data;
    using emu::util::byte::to_u16;
    using emu::util::file::read_file;
    using emu::util::string::split;

    LmcApplication::LmcApplication(const std::string &file) {
        load_file(file);
    }

    std::unique_ptr<Session> LmcApplication::new_session() {
        return std::make_unique<LmcApplicationSession>(m_loaded_file, m_memory);
    }

    void LmcApplication::load_file(const std::string &file) {
        m_loaded_file = file;

        const std::stringstream file_content = read_file(file);
        const std::vector<Data> code = Assembler::assemble(file_content);

        m_memory.add(code);
    }
}
