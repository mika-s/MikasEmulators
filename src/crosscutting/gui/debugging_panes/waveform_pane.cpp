#include "waveform_pane.h"
#include "audio/waveform.h"
#include "crosscutting/typedefs.h"
#include "debugging/debug_container.h"
#include "imgui.h"
#include <cstddef>
#include <fmt/core.h>
#include <string>
#include <utility>
#include <vector>

namespace emu::gui {

WaveformPane::WaveformPane() = default;

void WaveformPane::attach_debug_container(std::shared_ptr<DebugContainer<u16, u8, 16>> debug_container)
{
    m_debug_container = std::move(debug_container);
    m_is_debug_container_set = true;
}

void WaveformPane::draw(char const* title, bool* p_open)
{
    if (!ImGui::Begin(title, p_open, ImGuiWindowFlags_MenuBar)) {
        ImGui::End();
        return;
    }

    if (!m_is_debug_container_set) {
        ImGui::Text("The debug container is not provided this pane.");
    } else if (!m_debug_container->is_waveforms_set()) {
        ImGui::Text("The waveforms are not provided to this pane.");
    } else {
        unsigned int waveform_idx = 0;
        for (auto& waveform : m_debug_container->waveforms()) {
            const std::vector<u8> samples = waveform.samples();
            float samples_as_float[waveform.samples().size()]; // NOLINT
            float max = 0.0f;
            for (std::size_t sample_idx = 0; sample_idx < samples.size(); ++sample_idx) {
                const u8 sample = samples[sample_idx];
                samples_as_float[sample_idx] = static_cast<float>(sample);
                if (max < sample) {
                    max = sample;
                }
            }

            ImGui::PlotHistogram(
                fmt::format("{}", waveform_idx).c_str(),
                samples_as_float,
                IM_ARRAYSIZE(samples_as_float),
                0, nullptr, 0, max + 1, ImVec2(0, waveform_width));

            ++waveform_idx;
        }
    }

    ImGui::End();
}
}
