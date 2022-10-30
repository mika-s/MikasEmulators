#include <cassert>
#include <iostream>
#include "audio.h"

namespace emu::applications::pacman {

    using wsg3::Waveform;

    Audio::Audio(
            const std::vector<u8> &sound_rom1,
            const std::vector<u8> &sound_rom2
    )
            : m_sound_chip(Wsg3(load_waveforms_from_roms(sound_rom1, sound_rom2))),
              m_is_muted(false) {

        if (SDL_Init(SDL_INIT_AUDIO) != 0) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "error initializing SDL audio: %s", SDL_GetError());
            exit(1);
        }

        SDL_AudioSpec audio_spec{
                .freq = sdl_frequency,
                .format = AUDIO_S16SYS,
                .channels = 1,
                .silence = 0,
                .samples = 1024,
                .padding = 0,
                .size = 0,
                .callback = nullptr,
                .userdata = nullptr
        };
        m_audio_device = SDL_OpenAudioDevice(
                nullptr,
                0,
                &audio_spec,
                nullptr,
                0
        );
        if (m_audio_device == 0) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "error opening audio device: %s", SDL_GetError());
            exit(1);
        }
        SDL_PauseAudioDevice(m_audio_device, 0);
    }

    Audio::~Audio() {
        SDL_PauseAudioDevice(m_audio_device, 1);
        SDL_CloseAudioDevice(m_audio_device);
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
    }

    void Audio::handle_sound(bool is_sound_enabled, std::vector<Voice> &voices) {
        if (!is_sound_enabled) {
            return;
        }

        const std::vector<i16> buffer = m_sound_chip.next_tick(voices);

        for (int i = 0; i < samples_per_frame; i++) {
            const u16 sample = buffer[resampling_ratio * static_cast<float>(i)] * m_volume * (m_is_muted ? 0 : 1);
            SDL_QueueAudio(m_audio_device, &sample, 1 * sizeof(i16));
        }
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

    std::vector<Waveform> Audio::waveforms() {
        return m_sound_chip.waveforms();
    }

    std::vector<Waveform> Audio::load_waveforms_from_roms(
            const std::vector<u8> &sound_rom1,
            const std::vector<u8> &sound_rom2
    ) {
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

        return waveforms;
    }
}
