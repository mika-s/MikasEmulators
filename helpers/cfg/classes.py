#!/usr/bin/env python3

from enum import Enum
import itertools


class ConnectionType(Enum):
    JumpTrue = 'JT'
    JumpFalse = 'JF'
    CallTrue = 'CT'
    CallFalse = 'CF'
    ReturnTrue = 'RT'
    ReturnFalse = 'RF'
    NextBlock = 'NB'


class Instruction:
    def __init__(self, name: str, is_branching: bool):
        self.name = name
        self.is_branching = is_branching


class Language:
    def __init__(self,
                 jmp_instructions: list[Instruction],
                 call_instructions: list[Instruction],
                 ret_instructions: list[Instruction]):
        self.jmp_instructions = jmp_instructions
        self.call_instructions = call_instructions
        self.ret_instructions = ret_instructions


class DisassembledLine:
    def __init__(self, address: str, code: list[str]):
        self.is_leader: bool = False
        self.address: int = int(address, 16)
        self.code: list[str] = code
        self.instruction: str = code[0].strip()
        self.argument: str = code[1].strip() if len(code) > 1 else None

    def set_leader(self):
        self.is_leader = True

    def __str__(self):
        leader_str = 'L' if self.is_leader else ' '
        return f"{leader_str} {self.address:04x} {' '.join(self.code)}"

    @staticmethod
    def from_raw_string(raw_string: str):
        split = raw_string.split(' ')
        return DisassembledLine(split[0], split[1:])


class BasicBlock:
    _new_id = itertools.count()

    def __init__(self, lines: list[DisassembledLine]):
        self.id: int = next(self._new_id)
        self.lines: list[DisassembledLine] = lines
        self.successors: list[tuple[BasicBlock, ConnectionType]] = []
        self.is_entry_block: bool = False
        self.is_exit_block: bool = False

    def add_successor(self, connected_block: 'BasicBlock', connection_type: ConnectionType):
        self.successors.append((connected_block, connection_type))

    def line_from_address(self, address: int):
        for line in self.lines:
            if line.address == address:
                return line

        raise LookupError(f"Could not find {address:04x} in the basic block:\n{self}")

    def __str__(self):
        id_str = f'id: {self.id}'

        entry_exit = ''
        if self.is_entry_block and self.is_exit_block:
            entry_exit = ' (entry, exit)'
        elif self.is_entry_block:
            entry_exit = ' (entry)'
        elif self.is_exit_block:
            entry_exit = ' (exit)'

        connections_str = ', connected to: ' \
                          + ', '.join(map(lambda x: str(x[0].id) + " (" + str(x[1].value) + ")", self.successors))

        lines_str = '\n'.join(map(lambda x: str(x), self.lines))

        return id_str + entry_exit + connections_str + '\n' + lines_str
