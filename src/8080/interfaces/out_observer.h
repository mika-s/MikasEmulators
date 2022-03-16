#ifndef MIKA_EMULATORS_8080_INTERFACES_OUT_OBSERVER_H
#define MIKA_EMULATORS_8080_INTERFACES_OUT_OBSERVER_H

namespace emu::cpu8080 {

    class OutObserver {
    public:
        virtual ~OutObserver() = default;

        virtual void out_changed(std::uint8_t port) = 0;
    };
}

#endif //MIKA_EMULATORS_8080_INTERFACES_OUT_OBSERVER_H
