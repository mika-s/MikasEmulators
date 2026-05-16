#pragma once

#include "usings.h"
#include <string>
#include <unordered_map>

namespace emu::lmc {

class Environment {
public:
    void add_label(std::string const& label, Address address);

    auto get_address_given_label(std::string const& label) const -> Address;

private:
    std::unordered_map<std::string, Address> m_mapping;
};
}
