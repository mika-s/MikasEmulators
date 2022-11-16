#!/usr/bin/env python3

from classes import DisassembledLine


def parse_lines(raw_code, ignore_ranges=None):
    if ignore_ranges is None:
        ignore_ranges = []

    parsed_code = {}
    for line in raw_code:
        trimmed_line = line.strip()
        trimmed_line = trimmed_line.replace('\t\t', ' ')
        if len(trimmed_line) == 0:
            continue

        disassembled_line = DisassembledLine.from_raw_string(trimmed_line)
        is_getting_ignored = False
        for ignore_range in ignore_ranges:
            if ignore_range[0] <= disassembled_line.address <= ignore_range[1]:
                is_getting_ignored = True

        if not is_getting_ignored:
            parsed_code[disassembled_line.address] = disassembled_line

    return parsed_code


def main():
    with open('test_source_code.txt') as f:
        lines = f.readlines()

    disassembled_lines = parse_lines(lines, ignore_ranges=[(0x1a93, 0x1fff)])

    for disassembled_line in disassembled_lines.values():
        print(disassembled_line)


if __name__ == "__main__":
    main()
