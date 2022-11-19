#!/usr/bin/env python3

from blocks import find_blocks
from classes import BasicBlock, DisassembledLine, Language
from dead_code_removal import remove_dead_code
from graph import create_graph
from leaders import find_leaders
from parse import parse_lines
from rules import Cpu, get_rules


class FirstLineIsNotLeaderError(Exception):
    def __init__(self, basic_block: BasicBlock):
        super().__init__(
            f"The first line in a basic block has to be a leader\nBasic block:\n{basic_block}"
        )


class MoreThanOneLeaderError(Exception):
    def __init__(self, leaders: int, basic_block: BasicBlock):
        super().__init__(
            f"Found more than one leader in a basic block. Leaders: {leaders}\nBasic block:\n{basic_block}"
        )


class MoreThanOneBranchingInstructionError(Exception):
    def __init__(self, basic_block: BasicBlock):
        super().__init__(
            f"Found more than one branching instruction in a basic block.\nBasic block:\n{basic_block}"
        )


class DisassembledLineNotInBasicBlockError(Exception):
    def __init__(self, disassembled_line: DisassembledLine):
        super().__init__(
            f"Found a disassembled line that's not in a basic block.\nDisassembled line:\n{disassembled_line}"
        )


def validate_basic_blocks(basic_blocks: list[BasicBlock],
                          basic_blocks_before_dead_code_removal: list[BasicBlock],
                          disassembled_lines: dict[int, DisassembledLine],
                          language: Language):
    """
    The flow of control can only enter the basic block through
    the first instruction in the block.

    There are no jumps into the middle of the block.

    Control will leave the block without halting or branching,
    except possibly at the last instruction in the block.

    An instruction can only be in one block.
    All instructions have to be in a block (before dead-code removal).
    """

    def is_jump_instruction(instruction: str) -> bool:
        return instruction in map(lambda x: x.name, language.jmp_instructions)

    def is_call_instruction(instruction: str) -> bool:
        return instruction in map(lambda x: x.name, language.call_instructions)

    def is_return_instruction(instruction: str) -> bool:
        return instruction in map(lambda x: x.name, language.ret_instructions)

    def first_line_is_leader(_basic_block: BasicBlock):
        if not _basic_block.lines[0].is_leader:
            raise FirstLineIsNotLeaderError(_basic_block)

    def has_only_one_leader(_basic_block: BasicBlock):
        leaders = 0
        for instruction in _basic_block.lines:
            if instruction.is_leader:
                leaders += 1

        if leaders > 1:
            raise MoreThanOneLeaderError(leaders, _basic_block)

    def has_only_one_jmp_call_or_ret(_basic_block: BasicBlock):
        branching_blocks = 0
        for instruction in _basic_block.lines:
            if is_jump_instruction(instruction.instruction) \
                    or is_call_instruction(instruction.instruction) \
                    or is_return_instruction(instruction.instruction):
                branching_blocks += 1

        if branching_blocks > 1:
            raise MoreThanOneBranchingInstructionError(_basic_block)

    for basic_block in basic_blocks:
        first_line_is_leader(basic_block)
        has_only_one_leader(basic_block)
        has_only_one_jmp_call_or_ret(basic_block)

    address_to_block: dict[int, BasicBlock] = {}
    for block in basic_blocks_before_dead_code_removal:
        for line in block.lines:
            address = line.address
            if address in address_to_block:
                raise RuntimeError(f"Address in multiple blocks: {address}")

            address_to_block[address] = block

    for line in disassembled_lines.values():
        if line.address not in address_to_block:
            raise DisassembledLineNotInBasicBlockError(line)


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

    print('Validation ok')


if __name__ == "__main__":
    main()
