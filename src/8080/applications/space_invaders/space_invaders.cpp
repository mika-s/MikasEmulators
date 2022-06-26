#include "space_invaders.h"
#include "8080/cpu.h"
#include "crosscutting/util/byte_util.h"
#include "crosscutting/util/file_util.h"

namespace emu::cpu8080::applications::space_invaders {

    using emu::cpu8080::RunStatus;
    using emu::util::file::read_file_into_vector;

    SpaceInvaders::SpaceInvaders(
            const Settings &settings,
            std::shared_ptr<Gui> gui,
            std::shared_ptr<Input> input
    )
            : m_settings(settings),
              m_gui(std::move(gui)),
              m_input(std::move(input)) {
        load_file();
    }

    std::unique_ptr<Session> SpaceInvaders::new_session() {
        return std::make_unique<SpaceInvadersSession>(m_settings, m_gui, m_input, m_memory);
    }

    void SpaceInvaders::load_file() {
        m_memory.add(read_file_into_vector("invaders.h")); // $0000-$07ff: invaders.h
        m_memory.add(read_file_into_vector("invaders.g")); // $0800-$0fff: invaders.g
        m_memory.add(read_file_into_vector("invaders.f")); // $1000-$17ff: invaders.f
        m_memory.add(read_file_into_vector("invaders.e")); // $1800-$1fff: invaders.e
        m_memory.add(create_work_ram());                   // $2000-$23ff: work RAM
        m_memory.add(create_vram());                       // $2400-$3fff: video RAM
    }

    std::vector<u8> SpaceInvaders::create_work_ram() {
        std::vector<u8> work_ram;
        const int size = 0x23ff - 0x2000;

        work_ram.reserve(size);
        for (int i = 0; i <= size; ++i) {
            work_ram.push_back(0);
        }

        return work_ram;
    }

    std::vector<u8> SpaceInvaders::create_vram() {
        std::vector<u8> vram;
        const int size = 0x3fff - 0x2400;

        vram.reserve(size);
        for (int i = 0; i <= size; ++i) {
            vram.push_back(0);
        }

        return vram;
    }
}
