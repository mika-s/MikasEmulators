#include "space_invaders.h"
#include "crosscutting/util/file_util.h"
#include "gui_imgui.h"
#include "gui_sdl.h"
#include "input_imgui.h"
#include "input_sdl.h"

namespace emu::applications::space_invaders {

    using emu::i8080::RunStatus;
    using emu::util::file::read_file_into_vector;

    SpaceInvaders::SpaceInvaders(const Settings &settings, const GuiType gui_type)
        : m_settings(settings) {
        if (gui_type == GuiType::DEBUGGING) {
            m_gui = std::make_shared<GuiImgui>();
            m_input = std::make_shared<InputImgui>();
            m_startup_runstatus = RunStatus::PAUSED;
        } else {
            m_gui = std::make_shared<GuiSdl>();
            m_input = std::make_shared<InputSdl>();
            m_startup_runstatus = RunStatus::RUNNING;
        }

        load_file();
    }

    std::unique_ptr<Session> SpaceInvaders::new_session() {
        return std::make_unique<SpaceInvadersSession>(m_settings, m_startup_runstatus, m_gui, m_input, m_memory);
    }

    void SpaceInvaders::load_file() {
        const std::string directory = "roms/8080/space_invaders/";
        m_memory.add(read_file_into_vector(directory + "invaders.h")); // $0000-$07ff: invaders.h
        m_memory.add(read_file_into_vector(directory + "invaders.g")); // $0800-$0fff: invaders.g
        m_memory.add(read_file_into_vector(directory + "invaders.f")); // $1000-$17ff: invaders.f
        m_memory.add(read_file_into_vector(directory + "invaders.e")); // $1800-$1fff: invaders.e
        m_memory.add(create_work_ram());                               // $2000-$23ff: work RAM
        m_memory.add(create_vram());                                   // $2400-$3fff: video RAM
        m_memory.add(fill_remaining(m_memory.size()));
    }

    std::vector<u8> create_empty_vector(std::size_t size) {
        std::vector<u8> vec(size, 0);
        return vec;
    }

    std::vector<u8> SpaceInvaders::create_work_ram() {
        return create_empty_vector(0x23ff - 0x2000);
    }

    std::vector<u8> SpaceInvaders::create_vram() {
        return create_empty_vector(0x3fff - 0x2400);
    }

    std::vector<u8> SpaceInvaders::fill_remaining(std::size_t memory_size) {
        return create_empty_vector(UINT16_MAX - memory_size);
    }
}
