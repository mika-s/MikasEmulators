#ifndef MIKA_EMULATORS_CROSSCUTTING_MISC_NUMBER_H
#define MIKA_EMULATORS_CROSSCUTTING_MISC_NUMBER_H

#include "crosscutting/typedefs.h"
#include <cstddef>
#include <ostream>
#include <vector>

namespace emu::misc {

    void dummy();

    template<std::size_t M>
    class UInteger {
    public:
        explicit UInteger<M>(u64 value)
            : m_value(value % M) {
            dummy();
        }

        UInteger<M> operator+(const UInteger<M> rhs) const {
            const u64 new_value = (m_value + rhs.m_value) % M;
            return UInteger(new_value);
        }

        UInteger<M> &operator++() {
            m_value = (m_value + 1) % M;
            return *this;
        }

        UInteger<M> operator++(int) {
            UInteger<M> ret = *this;
            this->operator++();
            return ret;
        }

        UInteger<M> &operator+=(const UInteger<M> &rhs) {
            m_value = (m_value + rhs.m_value) % M;
            return *this;
        }

        UInteger<M> operator-(const UInteger<M> rhs) const {
            if (rhs.m_value > m_value) {
                u64 new_value = rhs.m_value - m_value;
                new_value = M - new_value;
                return UInteger(new_value);
            } else {
                return UInteger(m_value - rhs.m_value);
            }
        }

        UInteger<M> &operator--() {
            if (m_value == 0) {
                m_value = M - 1;
                return *this;
            } else {
                m_value = (m_value - 1);
            }

            return *this;
        }

        UInteger<M> operator--(int) {
            UInteger<M> ret = *this;
            this->operator--();
            return ret;
        }

        UInteger<M> &operator-=(const UInteger<M> &rhs) {
            if (rhs.m_value > m_value) {
                u64 new_value = rhs.m_value - m_value;
                new_value = M - new_value;
                m_value = new_value;
            } else {
                m_value -= rhs.m_value;
            }

            return *this;
        }

        bool operator==(const UInteger<M> &rhs) const {
            return m_value == rhs.m_value;
        }

        bool operator<=(const UInteger<M> &rhs) const {
            return m_value <= rhs.m_value;
        }

        bool operator>=(const UInteger<M> &rhs) const {
            return m_value >= rhs.m_value;
        }

        bool operator<(const UInteger<M> &rhs) const {
            return m_value < rhs.m_value;
        }

        bool operator>(const UInteger<M> &rhs) const {
            return m_value > rhs.m_value;
        }

        friend std::ostream &operator<<(std::ostream &os, const UInteger<M> &rhs) {
            os << rhs.m_value;
            return os;
        }

        explicit operator std::vector<u64>::size_type() {
            return m_value;
        }

        [[nodiscard]] u64 underlying() const {
            return m_value;
        }

    private:
        u64 m_value;
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_MISC_NUMBER_H
