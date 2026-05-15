#pragma once

#include "crosscutting/debugging/breakpoint.h"
#include "crosscutting/misc/uinteger.h"
#include <unordered_map>

namespace emu::debugger {

using emu::misc::UInteger;

struct KeyHasher {
    auto operator()(u8 const& k) const -> std::size_t // NOLINT(*-identifier-length)
    {
        return k;
    }

    auto operator()(u16 const& k) const -> std::size_t // NOLINT(*-identifier-length)
    {
        return k;
    }

    template<std::size_t M>
    auto operator()(UInteger<M> const& k) const -> std::size_t // NOLINT(*-identifier-length)
    {
        return k.underlying();
    }
};

template<class A, std::size_t B>
class Debugger {
public:
    void add_breakpoint(A breakpoint_address, Breakpoint<A, B> breakpoint)
    {
        m_breakpoints.insert_or_assign(breakpoint_address, std::move(breakpoint));
    }

    void remove_breakpoint(A breakpoint_address)
    {
        m_breakpoints.erase(breakpoint_address);
    }

    auto breakpoints() const -> std::unordered_map<A, Breakpoint<A, B>, KeyHasher> const&
    {
        return m_breakpoints;
    }

    void clear_all_breakpoints()
    {
        m_breakpoints.clear();
    }

    auto has_breakpoint(A breakpoint_address) -> bool
    {
        return m_breakpoints.contains(breakpoint_address);
    }

private:
    std::unordered_map<A, Breakpoint<A, B>, KeyHasher> m_breakpoints;
};
}
