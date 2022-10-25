#include <fmt/format.h>
#include <stdexcept>
#include "wsg3.h"

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

    /**
     * 1. Add the 20-bit (or 16-bit) voice frequency to the 20-bit (or 16-bit) accumulator.
     * 2. Use the waveform 0-7 to lookup a 32-byte sample in the Sound ROM.
     * 3. Take the top 5 bits of the accumulator to look up a nibble 0-31 in that sample.
     * 4. Multiply that nibble by the volume nibble 0-15.
     * 5. Send the result to the amplifier for output.
     */
    void Wsg3::next_tick(const std::vector<Voice> &voices) {
        if (voices.size() != expected_number_of_voices) {
            throw std::invalid_argument(
                    fmt::format(
                            "Expected number of voices is {}, but number of voices provided is {}",
                            expected_number_of_voices,
                            voices.size()
                    )
            );
        }
    }
}
