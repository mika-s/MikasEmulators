#ifndef MIKA_EMULATORS_CROSSCUTTING_MISC_GOVERNOR_H
#define MIKA_EMULATORS_CROSSCUTTING_MISC_GOVERNOR_H

#include <functional>
#include "crosscutting/typedefs.h"

namespace emu::misc {

    class Governor {
    public:
        Governor(double gain, long double limit, std::function<u64()> tick_retriever);

        bool is_time_to_update();

    private:
        double m_gain;
        u64 m_last_tick;
        long double m_limit;
        std::function<u64()> m_tick_retriever;
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_MISC_GOVERNOR_H
