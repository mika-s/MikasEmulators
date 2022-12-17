#include "stopped_state.h"
#include <utility>

namespace emu::applications::space_invaders {
class StateContext;
}

namespace emu::applications::space_invaders {

StoppedState::StoppedState(std::shared_ptr<StateContext> state_context)
    : m_ctx(std::move(state_context))
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
