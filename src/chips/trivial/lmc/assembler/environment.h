#pragma once

#include "usings.h"
#include <string>
#include <unordered_map>

namespace emu::lmc {

class Environment {
public:
    void add_label(std::string const& label, Address address);

    Address get_address_given_label(std::string const& label);

private:
    std::unordered_map<std::string, Address> m_mapping;
};
}
