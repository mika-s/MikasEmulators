#!/usr/bin/env python3

"""
This program is used for developing the advanced disassembler with
a control flow graph.

To be more specific, it's used for prototyping the basic block creation
algorithm in a higher-level language.
"""

from blocks import find_blocks
from dead_code_removal import remove_dead_code
from graph import create_graph
from leaders import find_leaders
from parse import parse_lines
from rules import Cpu, get_rules
from validator import validate_basic_blocks


def main():
    i8080_rules = get_rules(Cpu.I8080)

    with open('test_source_code.txt') as f:
        lines = f.readlines()

    disassembled_lines = parse_lines(lines, ignore_ranges=[(0x1a93, 0x1fff)])
    find_leaders(disassembled_lines, i8080_rules)
    basic_blocks = find_blocks(disassembled_lines)
    create_graph(basic_blocks, i8080_rules)
    basic_blocks_proper = remove_dead_code(basic_blocks)
    validate_basic_blocks(basic_blocks_proper, basic_blocks, disassembled_lines, i8080_rules)

    for basic_block in basic_blocks_proper:
        print(f'{basic_block}\n')


if __name__ == "__main__":
    main()
