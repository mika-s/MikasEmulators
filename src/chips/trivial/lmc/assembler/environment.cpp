#include "environment.h"
#include <fmt/core.h>
#include <stdexcept>

namespace emu::lmc {

void Environment::add_label(std::string const& label, Address address)
{
#ifdef __EMSCRIPTEN__
    if (m_mapping.count(label) > 0) {
#else
    if (m_mapping.contains(label)) {
#endif
        throw std::invalid_argument(
            fmt::format(
                "The {} label in the source code has already been assigned to an address: {}.",
                label,
                m_mapping.at(label).underlying()));
    }

    m_mapping.insert({ label, address });
}

Address Environment::get_address_given_label(std::string const& label)
{
#ifdef __EMSCRIPTEN__
    if (m_mapping.count(label) > 0) {
#else
    if (m_mapping.contains(label)) {
#endif
        throw std::invalid_argument(
            fmt::format("Could not find the {} label in the source code.", label));
    }
    return m_mapping.at(label);
}
}
