#!/usr/bin/env python3

"""
Runs two emulators in parallel and stops when the output from them differs.

The emulators have to print the contents of everything that should be
compared to stdout. The format of the print-out should be exactly the
same for both emulators. Each log line should end with '\n' (newline).

The emulators also have to respond to commands via stdin. This script expects
the emulators to send one log line at a time to stdout. When the script is ready
for a new log line (i.e. a new instruction to be executed) it will send 'n' and
then enter. It also expects the emulators to quit when sending 'q' and then
enter. The emulators have to flush to stdout instantly.

Do this before running this script:

- Put both emulators and their dependencies in the same directory.
- Set up the name and args to the emulator under test and reference emulator.
  The config is found in the emulator_under_test and reference_emulator objects.
- Set the log_line_token variable. It should be a unique string that is found in
  the log lines, and is used to identify the start of the log output.
- Make the emulators flush immediately when printing to stdout.
    * In C:   setbuf(stdout, NULL);
    * In C++: std::cout << "output" << std::flush;
- Make the emulators listen to commands on stdin:
    * Execute a new instruction when receiving 'n'.
    * Quit when receiving 'q'.
- The working directory of the script should be the same as the emulator binaries.

The script compares one instruction at a time. It compares by string comparison.
After comparing the emulator under test vs. the reference emulator, it will
ask the emulators to execute a new instruction if no difference was found.
It will quit and show the last 20 executed instructions if a diff was found
between them. For test binaries such as zexdoc it could take billions of
instructions before a diff is found.

If one or both of the emulators run to completion, i.e. no diffs are found, the
script will exit with exceptions etc. This is because it loses connection to the
emulator and there's no soft shutdown.

Tested on Python 3.8.8 only. Will need at least Python 3. Also, only tested on Linux.
"""

import asyncio


class Emulator:
    def __init__(self, name: str, args: []):
        self.name = name
        self.args = args

    def __str__(self):
        name = self.name
        maybe_space = ' ' if len(self.args) > 0 else ''
        args = ' '.join(self.args)

        return './' + name + maybe_space + args


# Must be adapted to each emulator:
emulator_under_test = Emulator('emulator', ['Z80', 'run', 'zexdoc'])
reference_emulator = Emulator('z80_tests', [])
log_line_token = 'pc=0x'

# Can be changed if you really want to:
queue_length = 20
log_string_encoding = 'utf-8'
new_instruction_character = b"n\n"


async def setup(sut_config: Emulator, ref_config: Emulator):
    sut = await asyncio.subprocess.create_subprocess_exec(
        f'./{sut_config.name}', *sut_config.args,
        stdin=asyncio.subprocess.PIPE,
        stdout=asyncio.subprocess.PIPE
    )
    ref = await asyncio.subprocess.create_subprocess_exec(
        f'./{ref_config.name}', *ref_config.args,
        stdin=asyncio.subprocess.PIPE,
        stdout=asyncio.subprocess.PIPE
    )

    return sut, ref


async def get_to_first_line(emu_process: asyncio.subprocess.Process):
    binary_line = await emu_process.stdout.readuntil(b"\n")

    while binary_line:
        str_line = binary_line.decode(log_string_encoding).strip("\n")
        if log_line_token in str_line:
            return str_line
        else:
            emu_process.stdin.write(new_instruction_character)

        binary_line = await emu_process.stdout.readuntil(b"\n")


def print_arrow_at_first_diff(sut_line, ref_line, offset):
    diff_pos = 0
    for pos in range(0, len(sut_line)):
        if sut_line[pos] == ref_line[pos]:
            diff_pos += 1
        else:
            break

    print(' ' * (diff_pos + offset) + '^')


async def compare_emulator_outputs(sut, sut_line, ref, ref_line):
    sut.stdin.write(new_instruction_character)
    ref.stdin.write(new_instruction_character)
    queue = []
    line_number = 0

    while sut_line == ref_line:
        if len(queue) < queue_length:
            queue.append(sut_line)
        else:
            queue.append(sut_line)
            queue.pop(0)

        if "pc=0x" not in sut_line:
            print(sut_line)

        sut_binary_line = await sut.stdout.readuntil(b"\n")
        sut_line = sut_binary_line.decode(log_string_encoding).strip("\n")
        sut.stdin.write(new_instruction_character)

        ref_binary_line = await ref.stdout.readuntil(b"\n")
        ref_line = ref_binary_line.decode(log_string_encoding).strip("\n")
        ref.stdin.write(new_instruction_character)

        line_number += 1

        if line_number % 1000000 == 0:
            print(f"At line number: {line_number:,}")

    print("********** FOUND DIFFERENCE IN THE EMULATORS **********")
    print(f"Line number: {line_number}\n\n")
    print(f"Last {queue_length} instructions before the diff:\n")
    for line in queue:
        print(line)

    print()
    print(f"SUT: {sut_line}")
    print(f"REF: {ref_line}")
    print_arrow_at_first_diff(sut_line, ref_line, 5)


async def main():
    sut, ref = await setup(emulator_under_test, reference_emulator)

    print(f"\nStarting comparison between '{emulator_under_test}' and '{reference_emulator}'")

    sut_line = await get_to_first_line(sut)
    ref_line = await get_to_first_line(ref)

    print("Found the first log lines of each emulator. Starting on comparison...\n\n")

    await compare_emulator_outputs(sut, sut_line, ref, ref_line)


asyncio.run(main())
