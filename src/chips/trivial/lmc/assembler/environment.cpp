#include "environment.h"
#include <fmt/core.h>
#include <stdexcept>

namespace emu::lmc {

    void Environment::add_label(const std::string &label, Address address) {
        if (m_mapping.contains(label)) {
            throw std::invalid_argument(
                    fmt::format(
                            "The {} label in the source code has already been assigned to an address: {}.",
                            label,
                            m_mapping.at(label).underlying()
                    )
            );
        }

        m_mapping.insert({label, address});
    }

    Address Environment::get_address_given_label(const std::string &label) {
        if (!m_mapping.contains(label)) {
            throw std::invalid_argument(
                    fmt::format("Could not find the {} label in the source code.", label)
            );
        }
        return m_mapping.at(label);
    }
}
