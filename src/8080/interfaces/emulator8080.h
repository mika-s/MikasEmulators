#ifndef MIKA_EMULATORS_8080_INTERFACES_EMULATOR_8080_H
#define MIKA_EMULATORS_8080_INTERFACES_EMULATOR_8080_H

namespace emu::cpu8080 {

    class Emulator8080 {
    public:
        virtual ~Emulator8080() = default;

        virtual void run() = 0;
    };
}

#endif //MIKA_EMULATORS_8080_INTERFACES_EMULATOR_8080_H
