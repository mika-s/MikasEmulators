#ifndef MIKA_EMULATORS_APPLICATIONS_PACMAN_AUDIO_H
#define MIKA_EMULATORS_APPLICATIONS_PACMAN_AUDIO_H

namespace emu::applications::pacman {

    class Audio {

    public:

        void mute();

        void unmute();

        void toggle_mute();

    private:

        bool m_is_muted;
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_PACMAN_AUDIO_H
