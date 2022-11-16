#!/usr/bin/env python3

from classes import BasicBlock
from leaders import find_leaders
from parse import parse_lines
from rules import Cpu, get_rules


def find_blocks(code):
    """
    2. Starting from a leader, the set of all following instructions until and not including the next leader is the
       basic block corresponding to the starting leader. Thus, every basic block has a leader.
    """

    basic_blocks = []
    buffer = []
    has_passed_first_line = False

    for code_line in code.values():
        if code_line.is_leader and has_passed_first_line:
            basic_blocks.append(BasicBlock(list(buffer)))
            buffer.clear()

        has_passed_first_line = True
        buffer.append(code_line)

    if len(buffer) != 0:
        basic_blocks.append(BasicBlock(buffer))

    return basic_blocks


def main():
    i8080_rules = get_rules(Cpu.I8080)

    with open('test_source_code.txt') as f:
        lines = f.readlines()

    disassembled_lines = parse_lines(lines, ignore_ranges=[(0x1a93, 0x1fff)])
    find_leaders(disassembled_lines, i8080_rules)
    basic_blocks = find_blocks(disassembled_lines)

    for basic_block in basic_blocks:
        print(f'{basic_block}\n')

    print(f'Number of blocks: {len(basic_blocks)}')


if __name__ == "__main__":
    main()
