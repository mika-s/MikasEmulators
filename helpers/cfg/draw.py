#!/usr/bin/env python3

import graphviz

from blocks import find_blocks
from classes import BasicBlock
from dead_code_removal import remove_dead_code
from graph import create_graph
from leaders import find_leaders
from parse import parse_lines
from rules import Cpu, get_rules


def create_graphviz_graph(basic_blocks: list[BasicBlock]):
    dot = graphviz.Digraph('cfg', comment='Control flow graph', graph_attr={'rankdir': 'TB'})
    dot.attr('node', shape='box')

    for basic_block in basic_blocks:
        instruction = ''
        for line in basic_block.lines:
            joined_code = ' '.join(line.code)
            instruction += f'{line.address:04x} {joined_code}\\l'

        dot.node(str(basic_block.id), instruction)

    for basic_block in basic_blocks:
        for successor in basic_block.successors:
            if basic_block.id == successor[0].id:
                dot.edge(str(basic_block.id), str(successor[0].id), dir='back')
            else:
                dot.edge(str(basic_block.id), str(successor[0].id))

    dot.render(directory='output').replace('\\', '/')


def main():
    i8080_rules = get_rules(Cpu.I8080)

    with open('test_source_code.txt') as f:
        lines = f.readlines()

    disassembled_lines = parse_lines(lines, ignore_ranges=[(0x1a93, 0x1fff)])
    find_leaders(disassembled_lines, i8080_rules)
    basic_blocks = find_blocks(disassembled_lines)
    create_graph(basic_blocks, i8080_rules)
    basic_blocks_proper = remove_dead_code(basic_blocks)
    create_graphviz_graph(basic_blocks_proper)


if __name__ == "__main__":
    main()
