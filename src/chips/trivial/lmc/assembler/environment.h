#ifndef MIKA_EMULATORS_CHIPS_LMC_ASSEMBLER_ENVIRONMENT_H
#define MIKA_EMULATORS_CHIPS_LMC_ASSEMBLER_ENVIRONMENT_H

#include "usings.h"
#include <string>
#include <unordered_map>

namespace emu::lmc {

    class Environment {
    public:
        void add_label(const std::string& label, Address address);

        Address get_address_given_label(const std::string& label);

    private:
        std::unordered_map<std::string, Address> m_mapping;
    };
}


#endif //MIKA_EMULATORS_CHIPS_LMC_ASSEMBLER_ENVIRONMENT_H
