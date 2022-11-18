#!/usr/bin/env python3

"""
This program is used for developing the advanced disassembler with
a control flow graph.

To be more specific, it's used for prototyping the basic block creation
algorithm in a higher-level language.



From Wikipedia: https://en.wikipedia.org/wiki/Basic_block
==========================================================

Input: A sequence of instructions (mostly three-address code).[7]
Output: A list of basic blocks with each three-address statement in exactly one block.

    1. Identify the leaders in the code. Leaders are instructions that come under any of the following 3 categories:
        1. It is the first instruction. The first instruction is a leader.
        2. The target of a conditional or an unconditional goto/jump instruction is a leader.
        3. The instruction that immediately follows a conditional or an unconditional goto/jump instruction is a leader.
    2. Starting from a leader, the set of all following instructions until and not including the next leader is the
       basic block corresponding to the starting leader. Thus, every basic block has a leader.

Instructions that end a basic block include the following:

* unconditional and conditional branches, both direct and indirect;
* returns to a calling procedure;
* instructions that may throw an exception;
* function calls can be at the end of a basic block if they can not return, such as functions that throw exceptions or
  special calls like C's longjmp and exit;
* the return instruction itself.

Instructions that begin a new basic block include the following:

* procedure and function entry points;
* targets of jumps or branches;
* "fall-through" instructions following some conditional branches;
* instructions following ones that throw exceptions;
* exception handlers.

Note that, because control can never pass through the end of a basic block, some block boundaries may have to be
modified after finding the basic blocks. In particular, fall-through conditional branches must be changed to two-way
branches, and function calls throwing exceptions must have unconditional jumps added after them. Doing these may require
adding labels to the beginning of other blocks.
"""

from blocks import find_blocks
from dead_code_removal import remove_dead_code
from graph import create_graph
from leaders import find_leaders
from parse import parse_lines
from rules import Cpu, get_rules


def main():
    i8080_rules = get_rules(Cpu.I8080)

    with open('test_source_code.txt') as f:
        lines = f.readlines()

    disassembled_lines = parse_lines(lines)
    find_leaders(disassembled_lines, i8080_rules)
    basic_blocks = find_blocks(disassembled_lines)
    create_graph(basic_blocks, i8080_rules)
    basic_blocks_proper = remove_dead_code(basic_blocks)

    for basic_block in basic_blocks_proper:
        print(f'{basic_block}\n')


if __name__ == "__main__":
    main()
