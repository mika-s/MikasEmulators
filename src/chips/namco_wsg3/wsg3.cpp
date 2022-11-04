#include "wsg3.h"
#include "voice.h"
#include <fmt/core.h>
#include <memory>
#include <stdexcept>
#include <utility>

namespace emu::wsg3 {

    Wsg3::Wsg3(std::vector<Waveform> waveforms)
        : m_waveforms(std::move(waveforms)) {

        if (m_waveforms.size() != expected_number_of_waveforms) {
            throw std::invalid_argument(
                    fmt::format(
                            "Expected number of waveforms is {}, but number of waveforms provided is {}",
                            expected_number_of_waveforms,
                            m_waveforms.size()
                    )
            );
        }
    }

    std::vector<Waveform> Wsg3::waveforms() {
        return m_waveforms;
    }

    std::vector<i16> Wsg3::next_tick(std::vector<Voice> &voices) {
        if (voices.size() != expected_number_of_voices) {
            throw std::invalid_argument(
                    fmt::format(
                            "Expected number of voices is {}, but number of voices provided is {}",
                            expected_number_of_voices,
                            voices.size()
                    )
            );
        }

        std::vector<i16> buffer;
        for (unsigned int i = 0; i < buffer_size; ++i) {
            i16 sample = 0;
            for (auto &voice: voices) {
                voice.accumulator((voice.frequency() + voice.accumulator()) & mask_for_20_bit);
                const u32 sample_idx = voice.accumulator() >> 15;
                i16 voice_sample = m_waveforms[voice.waveform_number()].samples()[sample_idx];
                voice_sample *= voice.volume();
                sample += voice_sample;
            }
            buffer.push_back(sample);
        }

        return buffer;
    }
}
