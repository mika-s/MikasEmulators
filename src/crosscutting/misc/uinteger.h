#ifndef MIKA_EMULATORS_CROSSCUTTING_MISC_NUMBER_H
#define MIKA_EMULATORS_CROSSCUTTING_MISC_NUMBER_H

#include "crosscutting/typedefs.h"
#include <cstddef>

namespace emu::misc {

    void dummy();

    template<std::size_t M>
    class UInteger {
    public:
        explicit UInteger<M>(u64 value)
            : m_value(value % M) {
            dummy();
        }

        UInteger operator+(const UInteger<M> rhs) const {
            const u64 new_value = (m_value + rhs.m_value) % M;
            return UInteger(new_value);
        }

        UInteger operator-(const UInteger<M> rhs) const {
            if (rhs.m_value > m_value) {
                u64 new_value = rhs.m_value - m_value;
                new_value = M - new_value;
                return UInteger(new_value);
            } else {
                return UInteger(m_value - rhs.m_value);
            }
        }

        [[nodiscard]] u64 underlying() const {
            return m_value;
        }

    private:
        u64 m_value;
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_MISC_NUMBER_H
