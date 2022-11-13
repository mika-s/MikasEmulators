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


i8080_rules = Language(
    [Instruction('JMP', False), Instruction('JZ', True), Instruction('JNZ', True), Instruction('JC', True),
     Instruction('JNC', True), Instruction('JPE', True), Instruction('JPO', True), Instruction('JP', True),
     Instruction('JM', True)],
    [Instruction('CALL', False), Instruction('CZ', True), Instruction('CNZ', True), Instruction('CC', True),
     Instruction('CNC', True), Instruction('CPE', True), Instruction('CPO', True), Instruction('CP', True),
     Instruction('CM', True)],
    [Instruction('RET', False), Instruction('RZ', True), Instruction('RNZ', True), Instruction('RC', True),
     Instruction('RNC', True), Instruction('RPE', True), Instruction('RPO', True), Instruction('RP', True),
     Instruction('RM', True)]
)


class DisassembledLine:
    def __init__(self, address, code):
        self.is_leader = False
        self.address = int(address, 16)
        self.code = code
        self.instruction = code[0].strip()
        self.argument = code[1].strip() if len(code) > 1 else None
        self.links_to = []

    def set_leader(self):
        self.is_leader = True

    def add_link_to(self, address):
        self.links_to.append(address)

    def __str__(self):
        leader_str = 'L' if self.is_leader else ' '
        links_to_str = (' (' + ', '.join(self.links_to) + ')') if len(self.links_to) > 0 else ''
        return f"{leader_str} {self.address:04x} {' '.join(self.code)}{links_to_str}"

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


def parse_lines(raw_code):
    parsed_code = {}
    for line in raw_code:
        trimmed_line = line.strip()
        trimmed_line = trimmed_line.replace('\t\t', ' ')
        if len(trimmed_line) == 0:
            continue

        disassembled_line = DisassembledLine.from_raw_string(trimmed_line)
        parsed_code[disassembled_line.address] = disassembled_line

    return parsed_code


def find_leaders(code, language):
    """
    1. Identify the leaders in the code. Leaders are instructions that come under any of the following 3 categories:
      1. It is the first instruction. The first instruction is a leader.
      2. The target of a conditional or an unconditional goto/jump instruction is a leader.
      3. The instruction that immediately follows a conditional or an unconditional goto/jump instruction is a leader.
    """

    def is_jump_instruction(instruction):
        return instruction in map(lambda x: x.name, language.jmp_instructions)

    def is_return_instruction(instruction):
        return instruction in map(lambda x: x.name, language.ret_instructions)

    future_leaders = set()  # for jumps forward

    line_no = 1
    is_previous_instruction_a_jump = False

    for code_line in code.values():
        if line_no == 1:
            code_line.set_leader()
            is_previous_instruction_a_jump = False

        if is_previous_instruction_a_jump:
            code_line.set_leader()
            is_previous_instruction_a_jump = False

        if code_line.address in future_leaders:
            code[code_line.address].set_leader()
            future_leaders.remove(code_line.address)
            is_previous_instruction_a_jump = False

        if is_jump_instruction(code_line.instruction):
            address_in_argument = int(code_line.argument, 16)
            if address_in_argument in code:
                code[address_in_argument].set_leader()
            else:
                future_leaders.add(address_in_argument)

            is_previous_instruction_a_jump = True
        elif is_return_instruction(code_line.instruction):
            is_previous_instruction_a_jump = True

        line_no += 1


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


def create_graph(basic_blocks, language):
    """
    Create a control flow graph given the basic blocks.
    """

    def find_next_address(address_to_block, address):
        """
        Look for the next address after address.
        """

        address += 1
        while address not in address_to_block:
            address += 1

        return address

    basic_blocks[0].is_entry_block = True
    basic_blocks[-1].is_exit_block = True

    # Set up map of address to basic block
    address_to_block = {}
    for block in basic_blocks:
        for line in block.lines:
            address = line.address
            if address in address_to_block:
                raise RuntimeError(f"Address in multiple blocks: {address}")

            address_to_block[address] = block

    # Connect jumps to other blocks
    for block in basic_blocks:
        for line in block.lines:
            if line.instruction in map(lambda x: x.name, language.jmp_instructions):
                jmp_instruction = [x for x in language.jmp_instructions if line.instruction == x.name][0]
                if jmp_instruction.is_branching:
                    address = line.address
                    block.add_successor(address_to_block[find_next_address(address_to_block, address)])

                argument_address = int(line.argument, 16)
                block.add_successor(address_to_block[argument_address])

    # Connect calls to other blocks
    for block in basic_blocks:
        for line in block.lines:
            if line.instruction in map(lambda x: x.name, language.call_instructions):
                call_instruction = [x for x in language.call_instructions if line.instruction == x.name][0]
                if call_instruction.is_branching:
                    address = line.address
                    block.add_successor(address_to_block[find_next_address(address_to_block, address)])

                argument_address = int(line.argument, 16)
                block.add_successor(address_to_block[argument_address])

    # Connect basic blocks that are not jumping or calling as last instruction
    for block in basic_blocks:
        if block.is_exit_block:
            break

        last_line = block.lines[-1]
        if last_line.instruction not in map(lambda x: x.name, language.jmp_instructions) \
                and last_line.instruction not in map(lambda x: x.name, language.ret_instructions):
            address = last_line.address
            block.add_successor(address_to_block[find_next_address(address_to_block, address)])


with open('test_source_code.txt') as f:
    lines = f.readlines()

disassembled_lines = parse_lines(lines)
find_leaders(disassembled_lines, i8080_rules)

print('After finding leaders:\n')
for disassembled_line in disassembled_lines.values():
    print(disassembled_line)

basic_blocks = find_blocks(disassembled_lines)

print('\n\n\n\n')
create_graph(basic_blocks, i8080_rules)
for basic_block in basic_blocks:
    print(f'{basic_block}\n')
