#!/usr/bin/env python3

from parse import parse_lines
from rules import Cpu, get_rules


def find_leaders(code, language):
    """
    1. Identify the leaders in the code. Leaders are instructions that come under any of the following 3 categories:
      1. It is the first instruction. The first instruction is a leader.
      2. The target of a conditional or an unconditional goto/jump instruction is a leader.
      3. The instruction that immediately follows a conditional or an unconditional goto/jump instruction is a leader.


    NOTE:

        - How to deal with branching returns?
            * Set next line leader and argument address leader?
        - How to deal with ordinary return?
            * Set next line leader. Cannot continue.
    """

    def is_jump_instruction(instruction):
        return instruction in map(lambda x: x.name, language.jmp_instructions)

    def is_call_instruction(instruction):
        return instruction in map(lambda x: x.name, language.call_instructions)

    def is_return_instruction(instruction):
        return instruction in map(lambda x: x.name, language.ret_instructions)

    line_no = 1
    is_setting_next_instruction_to_leader = False

    for code_line in code.values():
        if line_no == 1:
            code_line.set_leader()
            is_setting_next_instruction_to_leader = False

        if is_setting_next_instruction_to_leader:
            code_line.set_leader()
            is_setting_next_instruction_to_leader = False

        if is_jump_instruction(code_line.instruction):
            address_in_argument = int(code_line.argument, 16)
            code[address_in_argument].set_leader()

            is_setting_next_instruction_to_leader = True

        if is_call_instruction(code_line.instruction):
            address_in_argument = int(code_line.argument, 16)
            code[address_in_argument].set_leader()

            is_setting_next_instruction_to_leader = True  # next line is the returned-to instruction

        elif is_return_instruction(code_line.instruction):
            is_setting_next_instruction_to_leader = True

        line_no += 1


def main():
    i8080_rules = get_rules(Cpu.I8080)

    with open('test_source_code.txt') as f:
        lines = f.readlines()

    disassembled_lines = parse_lines(lines, ignore_ranges=[(0x1a93, 0x1fff)])
    find_leaders(disassembled_lines, i8080_rules)

    for disassembled_line in disassembled_lines.values():
        print(disassembled_line)


if __name__ == "__main__":
    main()
