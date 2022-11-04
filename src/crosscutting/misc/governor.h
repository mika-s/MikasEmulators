#ifndef MIKA_EMULATORS_CROSSCUTTING_MISC_GOVERNOR_H
#define MIKA_EMULATORS_CROSSCUTTING_MISC_GOVERNOR_H

#include <functional>

namespace emu::misc {

    class Governor {
    public:
        Governor(
                long double limit,
                std::function<long double()> tick_retriever
        );

        bool is_time_to_update();

    private:
        long double m_last_tick;
        long double m_limit;
        std::function<long double()> m_tick_retriever;
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_MISC_GOVERNOR_H
