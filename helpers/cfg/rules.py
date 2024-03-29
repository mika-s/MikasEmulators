#!/usr/bin/env python3

from enum import Enum
from classes import Language, Instruction


class Cpu(Enum):
    I8080 = 1
    Z80 = 2


rules = {
    Cpu.I8080: Language(
        [Instruction('JMP', False), Instruction('JZ', True), Instruction('JNZ', True), Instruction('JC', True),
         Instruction('JNC', True), Instruction('JPE', True), Instruction('JPO', True), Instruction('JP', True),
         Instruction('JM', True)],
        [Instruction('CALL', False), Instruction('CZ', True), Instruction('CNZ', True), Instruction('CC', True),
         Instruction('CNC', True), Instruction('CPE', True), Instruction('CPO', True), Instruction('CP', True),
         Instruction('CM', True)],
        [Instruction('RET', False), Instruction('RZ', True), Instruction('RNZ', True), Instruction('RC', True),
         Instruction('RNC', True), Instruction('RPE', True), Instruction('RPO', True), Instruction('RP', True),
         Instruction('RM', True)],
        [Instruction('RST', False)],
        [0x0000, 0x0008, 0x0010, 0x0018, 0x0020, 0x0028, 0x0030, 0x0038]
    )
}


def get_rules(cpu: Cpu):
    return rules[cpu]
