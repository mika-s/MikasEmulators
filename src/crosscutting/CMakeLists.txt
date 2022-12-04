set(SOURCES_CROSSCUTTING_CPP
        audio/waveform.cpp
        debugging/breakpoint.cpp
        debugging/debugger.cpp
        debugging/disassembled_line.cpp
        exceptions/invalid_program_arguments_exception.cpp
        exceptions/rom_file_not_found_exception.cpp
        exceptions/unrecognized_opcode_exception.cpp
        gui/debugging_panes/debug_log_pane.cpp
        gui/debugging_panes/disassembly_pane.cpp
        gui/debugging_panes/spritemap_pane.cpp
        gui/debugging_panes/tilemap_pane.cpp
        gui/debugging_panes/waveform_pane.cpp
        gui/graphics/color.cpp
        gui/graphics/framebuffer.cpp
        gui/graphics/palette.cpp
        gui/graphics/sprite.cpp
        gui/graphics/tile.cpp
        gui/main_panes/code_editor_pane.cpp
        gui/main_panes/terminal_pane.cpp
        logging/logger.cpp
        memory/emulator_memory.cpp
        misc/governor.cpp
        misc/sdl_counter.cpp
        misc/uinteger.cpp
        util/byte_util.cpp
        util/file_util.cpp
        util/gui_util.cpp
        util/string_util.cpp
        )

set(SOURCES_CROSSCUTTING_H
        typedefs.h
        audio/waveform.h
        debugging/breakpoint.h
        debugging/debugger.h
        debugging/debug_container.h
        debugging/disassembled_line.h
        exceptions/invalid_program_arguments_exception.h
        exceptions/rom_file_not_found_exception.h
        exceptions/unrecognized_opcode_exception.h
        gui/gui_type.h
        gui/debugging_panes/disassembly_pane.h
        gui/debugging_panes/cpu_info_pane.h
        gui/debugging_panes/debug_log_pane.h
        gui/debugging_panes/io_info_pane.h
        gui/debugging_panes/memory_editor_pane.h
        gui/debugging_panes/spritemap_pane.h
        gui/debugging_panes/tilemap_pane.h
        gui/debugging_panes/waveform_pane.h
        gui/graphics/color.h
        gui/graphics/framebuffer.h
        gui/graphics/palette.h
        gui/graphics/sprite.h
        gui/graphics/tile.h
        gui/main_panes/code_editor_pane.h
        gui/main_panes/terminal_pane.h
        logging/log_observer.h
        logging/logger.h
        memory/emulator_memory.h
        memory/memory_mapped_io.h
        memory/next_byte.h
        memory/next_word.h
        misc/emulator.h
        misc/governor.h
        misc/run_status.h
        misc/sdl_counter.h
        misc/session.h
        misc/uinteger.h
        util/byte_util.h
        util/file_util.h
        util/gui_util.h
        util/string_util.h
        )

add_library(Crosscutting STATIC ${SOURCES_CROSSCUTTING_CPP} ${SOURCES_CROSSCUTTING_H})
target_link_libraries(Crosscutting PRIVATE Glad SDL2::Main SDL2::Image ImGui Doctest)
target_include_directories(Crosscutting PUBLIC ../)

if(iwyu_path)
    set_property(TARGET Crosscutting PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
endif()

