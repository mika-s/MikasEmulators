#ifndef MIKA_EMULATORS_CROSSCUTTING_DEBUGGING_DEBUGGER_H
#define MIKA_EMULATORS_CROSSCUTTING_DEBUGGING_DEBUGGER_H

#include "crosscutting/debugging/breakpoint.h"
#include "crosscutting/misc/uinteger.h"
#include <unordered_map>

namespace emu::debugger {

    using emu::misc::UInteger;

    struct KeyHasher {
        std::size_t operator()(const u8 &k) const {
            return k;
        }

        std::size_t operator()(const u16 &k) const {
            return k;
        }

        template<std::size_t M>
        std::size_t operator()(const UInteger<M> &k) const {
            return k.underlying();
        }
    };

    template<class A, std::size_t B>
    class Debugger {
    public:
        void add_breakpoint(A breakpoint_address, Breakpoint<A, B> breakpoint) {
            m_breakpoints.insert_or_assign(breakpoint_address, std::move(breakpoint));
        }

        void remove_breakpoint(A breakpoint_address) {
            m_breakpoints.erase(breakpoint_address);
        }

        std::unordered_map<A, Breakpoint<A, B>, KeyHasher> &breakpoints() {
            return m_breakpoints;
        }

        void clear_all_breakpoints() {
            m_breakpoints.clear();
        }

        bool has_breakpoint(A breakpoint_address) {
            return m_breakpoints.find(breakpoint_address) != m_breakpoints.end();
        }

    private:
        std::unordered_map<A, Breakpoint<A, B>, KeyHasher> m_breakpoints;
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_DEBUGGING_DEBUGGER_H
