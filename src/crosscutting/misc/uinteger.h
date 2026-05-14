#pragma once

#include "crosscutting/typedefs.h"
#include <cstddef>
#include <cstdlib>
#include <iomanip>
#include <ostream>
#include <vector>

namespace emu::misc {

void dummy();

template<std::size_t M>
class UInteger {
public:
    explicit UInteger(u64 value)
        : m_value(value % M)
    {
        dummy();
    }

    auto operator+(UInteger const rhs) const -> UInteger
    {
        const u64 new_value = (m_value + rhs.m_value) % M;
        return UInteger(new_value);
    }

    auto operator++() -> UInteger&
    {
        m_value = (m_value + 1) % M;
        return *this;
    }

    auto operator++(int) -> UInteger
    {
        UInteger ret = *this;
        this->operator++();
        return ret;
    }

    auto operator+=(UInteger const& rhs) -> UInteger&
    {
        m_value = (m_value + rhs.m_value) % M;
        return *this;
    }

    auto operator-(UInteger const rhs) const -> UInteger
    {
        if (rhs.m_value > m_value) {
            u64 new_value = rhs.m_value - m_value;
            new_value = M - new_value;
            return UInteger(new_value);
        }

        return UInteger(m_value - rhs.m_value);
    }

    auto operator--() -> UInteger&
    {
        if (m_value == 0) {
            m_value = M - 1;
            return *this;
        }

        m_value = m_value - 1;

        return *this;
    }

    auto operator--(int) -> UInteger
    {
        UInteger ret = *this;
        this->operator--();
        return ret;
    }

    auto operator-=(UInteger const& rhs) -> UInteger&
    {
        if (rhs.m_value > m_value) {
            u64 new_value = rhs.m_value - m_value;
            new_value = M - new_value;
            m_value = new_value;
        } else {
            m_value -= rhs.m_value;
        }

        return *this;
    }

    auto operator==(UInteger const& rhs) const -> bool
    {
        return m_value == rhs.m_value;
    }

    auto operator<=(UInteger const& rhs) const -> bool
    {
        return m_value <= rhs.m_value;
    }

    auto operator>=(UInteger const& rhs) const -> bool
    {
        return m_value >= rhs.m_value;
    }

    auto operator<(UInteger const& rhs) const -> bool
    {
        return m_value < rhs.m_value;
    }

    auto operator>(UInteger const& rhs) const -> bool
    {
        return m_value > rhs.m_value;
    }

    /* From Stack Overflow: https://stackoverflow.com/a/1498561/8574934
     * By user Brad: https://stackoverflow.com/users/180638/brad
     * Under licence CC BY-SA 2.5: https://creativecommons.org/licenses/by-sa/2.5/ */
    static auto num_digits(int x) -> int
    {
        x = abs(x);
        return (x < 10 ? 1 : (x < 100 ? 2 : (x < 1000 ? 3 : (x < 10000 ? 4 : (x < 100000 ? 5 : (x < 1000000 ? 6 : (x < 10000000 ? 7 : (x < 100000000 ? 8 : (x < 1000000000 ? 9 : 10)))))))));
    }

    friend auto operator<<(std::ostream& os, UInteger const& rhs) -> std::ostream&
    {
        const int num = num_digits(static_cast<int>(M));
        os << std::setw(num) << std::setfill('0') << rhs.m_value;
        return os;
    }

    explicit operator std::vector<u64>::size_type() const {
        return m_value;
    }

    [[nodiscard]] auto underlying() const -> u64
    {
        return m_value;
    }

private:
    u64 m_value;
};
}
