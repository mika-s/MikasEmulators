#ifndef MIKA_EMULATORS_8080_INTERFACES_SESSION_H
#define MIKA_EMULATORS_8080_INTERFACES_SESSION_H

namespace emu::i8080 {

    class Session {
    public:
        virtual ~Session() = default;

        virtual void run() = 0;

        virtual void pause() = 0;

        virtual void stop() = 0;
    };
}

#endif //MIKA_EMULATORS_8080_INTERFACES_SESSION_H
