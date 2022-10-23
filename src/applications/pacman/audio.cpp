#include "audio.h"

namespace emu::applications::pacman {

    void Audio::mute() {
        m_is_muted = true;
    }

    void Audio::unmute() {
        m_is_muted = false;
    }

    void Audio::toggle_mute() {
        m_is_muted = !m_is_muted;
    }
}
