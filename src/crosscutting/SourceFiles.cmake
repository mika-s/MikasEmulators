set(SOURCES_CROSSCUTTING_CPP
        src/main.cpp
        src/crosscutting/debugging/breakpoint.cpp
        src/crosscutting/debugging/debugger.cpp
        src/crosscutting/debugging/debug_container.cpp
        src/crosscutting/debugging/disassembled_line.cpp
        src/crosscutting/exceptions/invalid_program_arguments_exception.cpp
        src/crosscutting/exceptions/unrecognized_opcode_exception.cpp
        src/crosscutting/gui/cpu_info.cpp
        src/crosscutting/gui/debug_log.cpp
        src/crosscutting/gui/disassembly_window.cpp
        src/crosscutting/gui/io_info.cpp
        src/crosscutting/gui/memory_editor_window.cpp
        src/crosscutting/logging/logger.cpp
        src/crosscutting/util/byte_util.cpp
        src/crosscutting/util/file_util.cpp
        src/crosscutting/util/string_util.cpp
        )

set(SOURCES_CROSSCUTTING_H
        src/crosscutting/typedefs.h
        src/crosscutting/debugging/breakpoint.h
        src/crosscutting/debugging/debugger.h
        src/crosscutting/debugging/debug_container.h
        src/crosscutting/debugging/disassembled_line.h
        src/crosscutting/exceptions/invalid_program_arguments_exception.h
        src/crosscutting/exceptions/unrecognized_opcode_exception.h
        src/crosscutting/gui/cpu_info.h
        src/crosscutting/gui/debug_log.h
        src/crosscutting/gui/disassembly_window.h
        src/crosscutting/gui/gui_type.h
        src/crosscutting/gui/io_info.h
        src/crosscutting/gui/memory_editor_window.h
        src/crosscutting/logging/log_observer.h
        src/crosscutting/logging/logger.h
        src/crosscutting/misc/next_byte.h
        src/crosscutting/misc/next_word.h
        src/crosscutting/util/byte_util.h
        src/crosscutting/util/file_util.h
        src/crosscutting/util/string_util.h
        )
