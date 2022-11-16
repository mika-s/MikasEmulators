#!/usr/bin/env python3

import itertools


class Instruction:
    def __init__(self, name, is_branching):
        self.name = name
        self.is_branching = is_branching


class Language:
    def __init__(self, jmp_instructions, call_instructions, ret_instructions):
        self.jmp_instructions = jmp_instructions
        self.call_instructions = call_instructions
        self.ret_instructions = ret_instructions


class DisassembledLine:
    def __init__(self, address, code):
        self.is_leader = False
        self.address = int(address, 16)
        self.code = code
        self.instruction = code[0].strip()
        self.argument = code[1].strip() if len(code) > 1 else None

    def set_leader(self):
        self.is_leader = True

    def __str__(self):
        leader_str = 'L' if self.is_leader else ' '
        return f"{leader_str} {self.address:04x} {' '.join(self.code)}"

    @staticmethod
    def from_raw_string(raw_string):
        split = raw_string.split(' ')
        return DisassembledLine(split[0], split[1:])


class BasicBlock:
    new_id = itertools.count()

    def __init__(self, lines):
        self.id = next(self.new_id)
        self.lines = lines
        self.successors = []
        self.is_entry_block = False
        self.is_exit_block = False

    def add_successor(self, connected_block):
        self.successors.append(connected_block)

    def __str__(self):
        id_str = f'id: {self.id}'

        entry_exit = ''
        if self.is_entry_block and self.is_exit_block:
            entry_exit = ' (entry, exit)'
        elif self.is_entry_block:
            entry_exit = ' (entry)'
        elif self.is_exit_block:
            entry_exit = ' (exit)'

        connections_str = ', connected to: ' + ', '.join(map(lambda x: str(x.id), self.successors))

        lines_str = '\n'.join(map(lambda x: str(x), self.lines))

        return id_str + entry_exit + connections_str + '\n' + lines_str
