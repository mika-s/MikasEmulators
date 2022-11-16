#!/usr/bin/env python3

import subprocess
import graphviz

from blocks import find_blocks
from graph import create_graph
from leaders import find_leaders
from parse import parse_lines
from rules import Cpu, get_rules


def create_graphviz_graph(basic_blocks):
    dot = graphviz.Digraph('cfg', comment='Control flow graph')
    dot.attr('node', shape='box')

    for basic_block in basic_blocks:
        instruction = ''
        for line in basic_block.lines:
            joined_code = ' '.join(line.code)
            instruction += f'{line.address:04x} {joined_code}\\l'

        dot.node(str(basic_block.id), instruction)

    for basic_block in basic_blocks:
        for successor in basic_block.successors:
            dot.edge(str(basic_block.id), str(successor.id))

    dot.render(directory='output').replace('\\', '/')


def main():
    i8080_rules = get_rules(Cpu.I8080)

    with open('test_source_code.txt') as f:
        lines = f.readlines()

    disassembled_lines = parse_lines(lines, ignore_ranges=[(0x1a93, 0x1fff)])
    find_leaders(disassembled_lines, i8080_rules)
    basic_blocks = find_blocks(disassembled_lines)
    create_graph(basic_blocks, i8080_rules)
    create_graphviz_graph(basic_blocks)


if __name__ == "__main__":
    main()
