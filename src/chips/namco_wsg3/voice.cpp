#include "voice.h"
#include <fmt/core.h>
#include <stdexcept>

namespace emu::wsg3 {
Voice::Voice()
    : m_waveform_number(0)
    , m_frequency(0)
    , m_volume(0)
    , m_accumulator(0)
{
}

u8 Voice::waveform_number() const
{
    return m_waveform_number;
}

void Voice::waveform_number(u8 waveform_number)
{
    if (waveform_number > waveforms_supported - 1) {
        throw std::invalid_argument(
            fmt::format(
                "Waveform number has to be between 0 and {}, but was {}",
                waveforms_supported - 1,
                waveform_number));
    }

    m_waveform_number = waveform_number;
}

u32 Voice::frequency() const
{
    return m_frequency;
}

void Voice::frequency(u32 frequency)
{
    if (frequency > max_frequency - 1) {
        throw std::invalid_argument(
            fmt::format(
                "Frequency has to be between 0 and {}, but was {}",
                max_frequency - 1,
                frequency));
    }

    m_frequency = frequency;
}

u8 Voice::volume() const
{
    return m_volume;
}

void Voice::volume(u8 volume)
{
    if (volume > volume_levels_supported - 1) {
        throw std::invalid_argument(
            fmt::format(
                "Volume has to be between 0 and {}, but was {}",
                volume_levels_supported - 1,
                volume));
    }

    m_volume = volume;
}

u32 Voice::accumulator() const
{
    return m_accumulator;
}

void Voice::accumulator(u32 accumulator)
{
    m_accumulator = accumulator;
}
}
