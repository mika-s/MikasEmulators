#!/usr/bin/env python3

from blocks import find_blocks
from classes import BasicBlock
from leaders import find_leaders
from graph import create_graph
from parse import parse_lines
from rules import Cpu, get_rules


def remove_dead_code(basic_blocks: list[BasicBlock]) -> list[BasicBlock]:
    """
    Use BFS to find all basic blocks in use.
    """

    is_used: dict[int, bool] = {}
    for basic_block in basic_blocks:
        is_used[basic_block.id] = False

    is_used[0] = True

    queue = [basic_block for basic_block in basic_blocks if basic_block.is_entry_block]

    while len(queue) > 0:
        current = queue.pop()
        for successor in current.successors:
            if not is_used[successor.id]:
                is_used[successor.id] = True
                queue.insert(0, successor)

    return [basic_block for basic_block in basic_blocks if is_used[basic_block.id]]


def main():
    i8080_rules = get_rules(Cpu.I8080)

    with open('test_source_code.txt') as f:
        lines = f.readlines()

    disassembled_lines = parse_lines(lines, ignore_ranges=[(0x1a93, 0x1fff)])
    find_leaders(disassembled_lines, i8080_rules)
    basic_blocks = find_blocks(disassembled_lines)
    create_graph(basic_blocks, i8080_rules)
    basic_blocks_proper = remove_dead_code(basic_blocks)

    for basic_block in basic_blocks_proper:
        print(f'{basic_block}\n')

    print(f'Number of blocks: {len(basic_blocks_proper)}')


if __name__ == "__main__":
    main()
