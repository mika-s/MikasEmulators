#include "stopped_state.h"

namespace emu::applications::pacman {
class StateManager;
}

namespace emu::applications::pacman {

StoppedState::StoppedState(StateManager& state_manager)
    : m_state_manager(state_manager)
{
}

bool StoppedState::is_exit_state()
{
    return true;
}

void StoppedState::transition_to_run()
{
}

void StoppedState::transition_to_pause()
{
}

void StoppedState::transition_to_stop()
{
}

void StoppedState::transition_to_step()
{
}

void StoppedState::perform([[maybe_unused]] cyc& cycles)
{
}

}
