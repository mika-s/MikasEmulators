#!/usr/bin/env python3

# The flow of control can only
# enter the basic block through
# the first instruction in the
# block.
#
# There are no jumps into the
# middle of the block.
#
# --> First line must be a leader.
# --> Only the first line can be a leader
# --> CALL must create a new leader
#


# Control will leave the block
# without halting or branching,
# except possibly at the last
# instruction in the block.
#
# --> Can only leave the block at the end
#


# An instruction can only be in one block.
# All instructions have to be in a block (before dead-code removal).
