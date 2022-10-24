#include <cassert>
#include <iostream>
#include "audio.h"

namespace emu::applications::pacman {

    using wsg3::Waveform;

    Audio::Audio(
            const std::vector<u8> &sound_rom1,
            const std::vector<u8> &sound_rom2
    )
            : m_sound_chip(load_waveforms_from_roms(sound_rom1, sound_rom2)) {
    }


    void Audio::handle_sound(bool is_sound_enabled, const std::vector<Voice> &voices) {
        if (!is_sound_enabled) {
            return;
        }

        m_sound_chip.next_tick(voices);
    }

    void Audio::mute() {
        m_is_muted = true;
    }

    void Audio::unmute() {
        m_is_muted = false;
    }

    void Audio::toggle_mute() {
        m_is_muted = !m_is_muted;
    }

    Wsg3 Audio::load_waveforms_from_roms(const std::vector<u8> &sound_rom1, const std::vector<u8> &sound_rom2) {
        std::vector<Waveform> waveforms;
        std::vector<u8> samples;

        for (u8 byte: sound_rom1) {
            samples.push_back(byte);
            if (samples.size() == samples_per_waveform) {
                waveforms.emplace_back(samples);
                samples.clear();
            }
        }

        assert(waveforms.size() == 8);

        for (u8 byte: sound_rom2) {
            samples.push_back(byte);
            if (samples.size() == samples_per_waveform) {
                waveforms.emplace_back(samples);
                samples.clear();
            }
        }

        assert(waveforms.size() == 16);

        return Wsg3(waveforms);
    }
}
