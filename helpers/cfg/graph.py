#!/usr/bin/env python3

from blocks import find_blocks
from classes import BasicBlock, DisassembledLine, ConnectionType, Language
from leaders import find_leaders
from parse import parse_lines
from rules import Cpu, get_rules


def create_graph(basic_blocks: list[BasicBlock], language: Language):
    """
    Create a control flow graph given a list of basic blocks.
    """

    def is_jump_instruction(instruction: str) -> bool:
        return instruction in map(lambda x: x.name, language.jmp_instructions)

    def is_call_instruction(instruction: str) -> bool:
        return instruction in map(lambda x: x.name, language.call_instructions)

    def is_return_instruction(instruction: str) -> bool:
        return instruction in map(lambda x: x.name, language.ret_instructions)

    def find_next_address(_address_to_block: dict[int, BasicBlock], _address: int) -> int:
        """
        Look for the next address after address.
        """

        _address += 1
        while _address not in _address_to_block:
            _address += 1

        return _address

    basic_blocks[0].is_entry_block = True
    basic_blocks[-1].is_exit_block = True  # TODO: Probably not true

    def handle_returns(from_block: BasicBlock, to_block: BasicBlock, _address_to_block: dict[int, BasicBlock]):
        """
        Link the end of a function back to the next address after the caller's address.

        BFS is used, as the code can branch all over the place.
        """

        is_visited: set[int] = set()
        queue: list[DisassembledLine] = [from_block.lines[0]]

        while len(queue) > 0:
            current = queue.pop()
            successors: list[DisassembledLine] = []

            if is_return_instruction(current.instruction):
                _ret_instruction = [x for x in language.ret_instructions if current.instruction == x.name][0]
                if _ret_instruction.is_branching:
                    successor_address = find_next_address(_address_to_block, current.address)
                    successors.append(_address_to_block[successor_address].line_from_address(successor_address))

                _address_to_block[current.address].add_successor(to_block, ConnectionType.ReturnTrue)
            elif is_call_instruction(current.instruction):
                _call_instruction = [x for x in language.call_instructions if current.instruction == x.name][0]
                if call_instruction.is_branching:
                    successor_address = find_next_address(_address_to_block, current.address)
                    successors.append(_address_to_block[successor_address].line_from_address(successor_address))
            elif is_jump_instruction(current.instruction):
                _jmp_instruction = [x for x in language.jmp_instructions if current.instruction == x.name][0]
                if _jmp_instruction.is_branching:
                    _address = find_next_address(_address_to_block, current.address)
                    successors.append(_address_to_block[_address].line_from_address(_address))

                _argument_address = int(current.argument, 16)
                successors.append(_address_to_block[_argument_address].line_from_address(_argument_address))
            else:
                successor_address = find_next_address(_address_to_block, current.address)
                successors.append(_address_to_block[successor_address].line_from_address(successor_address))

            for successor in successors:
                if successor.address not in is_visited:
                    is_visited.add(successor.address)
                    queue.insert(0, successor)

    # Set up dictionary of address to basic block
    address_to_block: dict[int, BasicBlock] = {}
    for block in basic_blocks:
        for line in block.lines:
            address = line.address
            if address in address_to_block:
                raise RuntimeError(f"Address in multiple blocks: {address}")

            address_to_block[address] = block

    # Connect jumps to other blocks
    for block in basic_blocks:
        for line in block.lines:
            if is_jump_instruction(line.instruction):
                jmp_instruction = [x for x in language.jmp_instructions if line.instruction == x.name][0]
                if jmp_instruction.is_branching:
                    address = line.address
                    block.add_successor(
                        address_to_block[find_next_address(address_to_block, address)],
                        ConnectionType.JumpFalse
                    )

                argument_address = int(line.argument, 16)
                block.add_successor(address_to_block[argument_address], ConnectionType.JumpTrue)

    # Connect calls to other blocks
    for block in basic_blocks:
        for line in block.lines:
            if is_call_instruction(line.instruction):
                call_instruction = [x for x in language.call_instructions if line.instruction == x.name][0]
                if call_instruction.is_branching:
                    address = line.address
                    block.add_successor(
                        address_to_block[find_next_address(address_to_block, address)],
                        ConnectionType.CallFalse
                    )

                argument_address = int(line.argument, 16)
                block.add_successor(address_to_block[argument_address], ConnectionType.CallTrue)

                handle_returns(address_to_block[argument_address],
                               address_to_block[find_next_address(address_to_block, line.address)],
                               address_to_block)

    # Connect returns to next block if it is conditional
    for block in basic_blocks:
        for line in block.lines:
            if is_return_instruction(line.instruction):
                ret_instruction = [x for x in language.ret_instructions if line.instruction == x.name][0]
                if ret_instruction.is_branching:
                    address = line.address
                    block.add_successor(
                        address_to_block[find_next_address(address_to_block, address)],
                        ConnectionType.ReturnFalse
                    )

    # Connect basic blocks that are not jumping, calling or returning as last instruction
    for block in basic_blocks:
        if block.is_exit_block:
            break

        last_line = block.lines[-1]
        if not is_jump_instruction(last_line.instruction) \
                and not is_call_instruction(last_line.instruction) \
                and not is_return_instruction(last_line.instruction):
            address = last_line.address
            block.add_successor(
                address_to_block[find_next_address(address_to_block, address)],
                ConnectionType.NextBlock
            )


def main():
    i8080_rules = get_rules(Cpu.I8080)

    with open('test_source_code.txt') as f:
        lines = f.readlines()

    disassembled_lines = parse_lines(lines, ignore_ranges=[(0x1a93, 0x1fff)])
    find_leaders(disassembled_lines, i8080_rules)
    basic_blocks = find_blocks(disassembled_lines)
    create_graph(basic_blocks, i8080_rules)

    for basic_block in basic_blocks:
        print(f'{basic_block}\n')


if __name__ == "__main__":
    main()
