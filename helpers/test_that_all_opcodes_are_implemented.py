#!/usr/bin/env python3

"""
A script that runs through a list of instructions and checks
whether all the instructions are listed in the cpu.cpp and
disassemblerXY.cpp files.

Should be run with the helpers directory as working directory.

Tested on Python 3.8.8 only. Will need at least Python 3. Also, only tested on Linux.
"""

import re

instruction_marker_in_cpu_file = 'case '


def check_missing_in_file(file_for_maybe_missing, file_with_instructions):
    instructions_in_instruction_file = []
    with open(file_with_instructions) as f:
        for line in f.readlines():
            result = re.search(r"constexpr unsigned int ([A-Za-z0-9_]+).*\n", line)
            if result is not None:
                instructions_in_instruction_file.append(result.group(1))

    instructions_in_cpu_file = []
    with open(file_for_maybe_missing) as f:
        for line in f.readlines():
            result = re.search(r"case ([A-Za-z0-9_]+):.*\n", line)
            if result is not None:
                instructions_in_cpu_file.append(result.group(1))

    for instruction_in_instruction_file in instructions_in_instruction_file:
        if instruction_in_instruction_file not in instructions_in_cpu_file:
            print("\t - " + instruction_in_instruction_file)


print("8080 (cpu.cpp):")
check_missing_in_file('../src/chips/8080/cpu.cpp', '../src/chips/8080/instructions/instructions.h')
print()
print("8080 (disassembler8080.cpp):")
check_missing_in_file('../src/chips/8080/disassembler.cpp', '../src/chips/8080/instructions/instructions.h')

print("\n\n")
print("Z80 (cpu.cpp):")
check_missing_in_file('../src/chips/z80/cpu.cpp', '../src/chips/z80/instructions/instructions.h')
print()
print("Z80 (disassemblerZ80.cpp):")
check_missing_in_file('../src/chips/z80/disassembler.cpp', '../src/chips/z80/instructions/instructions.h')
