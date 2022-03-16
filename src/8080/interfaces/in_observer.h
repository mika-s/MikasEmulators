#ifndef MIKA_EMULATORS_8080_INTERFACES_IN_OBSERVER_H
#define MIKA_EMULATORS_8080_INTERFACES_IN_OBSERVER_H

namespace emu::cpu8080 {

    class InObserver {
    public:
        virtual ~InObserver() = default;

        virtual void in_requested(std::uint8_t port) = 0;
    };
}

#endif //MIKA_EMULATORS_8080_INTERFACES_IN_OBSERVER_H
