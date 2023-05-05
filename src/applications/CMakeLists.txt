set(SOURCES_APPLICATIONS_CPP
        frontend.cpp
        options.cpp

        command_line_arguments/command_line_arguments.cpp
        command_line_arguments/identifier.cpp
        command_line_arguments/long_option.cpp
        command_line_arguments/parser.cpp
        command_line_arguments/scanner.cpp
        command_line_arguments/short_option.cpp
        command_line_arguments/token.cpp

        cpm_8080/cpm_application.cpp
        cpm_8080/cpm_application_session.cpp
        cpm_8080/usage.cpp

        cpm_z80/cpm_application.cpp
        cpm_z80/cpm_application_session.cpp
        cpm_z80/usage.cpp

        game_boy/audio.cpp
        game_boy/gui.cpp
        game_boy/gui_imgui.cpp
        game_boy/gui_sdl.cpp
        game_boy/input_imgui.cpp
        game_boy/input_sdl.cpp
        game_boy/memory_mapped_io_for_pacman.cpp
        game_boy/pacman.cpp
        game_boy/pacman_session.cpp
        game_boy/settings.cpp
        game_boy/usage.cpp
        game_boy/states/paused_state.cpp
        game_boy/states/running_state.cpp
        game_boy/states/stepping_state.cpp
        game_boy/states/stopped_state.cpp
        game_boy/states/state_context.cpp

        lmc_application/gui_imgui.cpp
        lmc_application/input_imgui.cpp
        lmc_application/lmc_application.cpp
        lmc_application/lmc_application_session.cpp
        lmc_application/tui_terminal.cpp
        lmc_application/lmc_memory_editor.cpp
        lmc_application/usage.cpp
        lmc_application/states/paused_state.cpp
        lmc_application/states/running_state.cpp
        lmc_application/states/running_awaiting_input_state.cpp
        lmc_application/states/stepping_state.cpp
        lmc_application/states/stopped_state.cpp
        lmc_application/states/state_context.cpp

        pacman/audio.cpp
        pacman/gui.cpp
        pacman/gui_imgui.cpp
        pacman/gui_sdl.cpp
        pacman/input_imgui.cpp
        pacman/input_sdl.cpp
        pacman/memory_mapped_io_for_pacman.cpp
        pacman/pacman.cpp
        pacman/pacman_session.cpp
        pacman/settings.cpp
        pacman/usage.cpp
        pacman/states/paused_state.cpp
        pacman/states/running_state.cpp
        pacman/states/stepping_state.cpp
        pacman/states/stopped_state.cpp
        pacman/states/state_context.cpp

        space_invaders/audio.cpp
        space_invaders/cpu_io.cpp
        space_invaders/gui_imgui.cpp
        space_invaders/gui_sdl.cpp
        space_invaders/input_imgui.cpp
        space_invaders/input_sdl.cpp
        space_invaders/memory_map_for_space_invaders.cpp
        space_invaders/space_invaders.cpp
        space_invaders/space_invaders_session.cpp
        space_invaders/settings.cpp
        space_invaders/usage.cpp
        space_invaders/states/paused_state.cpp
        space_invaders/states/running_state.cpp
        space_invaders/states/stepping_state.cpp
        space_invaders/states/stopped_state.cpp
        space_invaders/states/state_context.cpp

        zxspectrum_48k/audio.cpp
        zxspectrum_48k/cpu_io.cpp
        zxspectrum_48k/gui.cpp
        zxspectrum_48k/gui_imgui.cpp
        zxspectrum_48k/gui_sdl.cpp
        zxspectrum_48k/input_imgui.cpp
        zxspectrum_48k/input_sdl.cpp
        zxspectrum_48k/keyboard_pane.cpp
        zxspectrum_48k/memory_map_for_zxspectrum_48k.cpp
        zxspectrum_48k/zxspectrum_48k.cpp
        zxspectrum_48k/zxspectrum_48k_print_header_session.cpp
        zxspectrum_48k/zxspectrum_48k_session.cpp
        zxspectrum_48k/settings.cpp
        zxspectrum_48k/usage.cpp
        zxspectrum_48k/formats/z80_format.cpp
        zxspectrum_48k/states/paused_state.cpp
        zxspectrum_48k/states/running_state.cpp
        zxspectrum_48k/states/stepping_state.cpp
        zxspectrum_48k/states/stopped_state.cpp
        zxspectrum_48k/states/state_context.cpp
        )

set(SOURCES_APPLICATIONS_H
        frontend.h
        options.h

        command_line_arguments/command_line_arguments.h
        command_line_arguments/identifier.h
        command_line_arguments/long_option.h
        command_line_arguments/parser.h
        command_line_arguments/scanner.h
        command_line_arguments/short_option.h
        command_line_arguments/token.h
        command_line_arguments/token_kind.h

        cpm_8080/cpm_application.h
        cpm_8080/cpm_application_session.h
        cpm_8080/usage.h

        cpm_z80/cpm_application.h
        cpm_z80/cpm_application_session.h
        cpm_z80/usage.h

        game_boy/audio.h
        game_boy/gui.h
        game_boy/gui_imgui.h
        game_boy/gui_sdl.h
        game_boy/input_imgui.h
        game_boy/input_sdl.h
        game_boy/key_request.h
        game_boy/memory_mapped_io_for_pacman.h
        game_boy/pacman.h
        game_boy/pacman_session.h
        game_boy/settings.h
        game_boy/usage.h
        game_boy/interfaces/gui_observer.h
        game_boy/interfaces/input.h
        game_boy/interfaces/key_observer.h
        game_boy/interfaces/state.h
        game_boy/states/paused_state.h
        game_boy/states/running_state.h
        game_boy/states/stepping_state.h
        game_boy/states/stopped_state.h
        game_boy/states/state_context.h

        lmc_application/ui.h
        lmc_application/gui_imgui.h
        lmc_application/input_imgui.h
        lmc_application/lmc_application.h
        lmc_application/lmc_application_session.h
        lmc_application/lmc_memory_editor.h
        lmc_application/tui_terminal.h
        lmc_application/usage.h
        lmc_application/interfaces/ui_observer.h
        lmc_application/interfaces/input.h
        lmc_application/interfaces/state.h
        lmc_application/states/paused_state.h
        lmc_application/states/running_state.h
        lmc_application/states/running_awaiting_input_state.h
        lmc_application/states/stepping_state.h
        lmc_application/states/stopped_state.h
        lmc_application/states/state_context.h

        pacman/audio.h
        pacman/gui.h
        pacman/gui_imgui.h
        pacman/gui_sdl.h
        pacman/input_imgui.h
        pacman/input_sdl.h
        pacman/key_request.h
        pacman/memory_mapped_io_for_pacman.h
        pacman/pacman.h
        pacman/pacman_session.h
        pacman/settings.h
        pacman/usage.h
        pacman/interfaces/gui_observer.h
        pacman/interfaces/input.h
        pacman/interfaces/key_observer.h
        pacman/interfaces/state.h
        pacman/states/paused_state.h
        pacman/states/running_state.h
        pacman/states/stepping_state.h
        pacman/states/stopped_state.h
        pacman/states/state_context.h

        space_invaders/audio.h
        space_invaders/cpu_io.h
        space_invaders/gui.h
        space_invaders/gui_imgui.h
        space_invaders/gui_io.h
        space_invaders/gui_sdl.h
        space_invaders/input_imgui.h
        space_invaders/input_sdl.h
        space_invaders/key_request.h
        space_invaders/memory_map_for_space_invaders.h
        space_invaders/sounds.h
        space_invaders/space_invaders.h
        space_invaders/space_invaders_session.h
        space_invaders/settings.h
        space_invaders/usage.h
        space_invaders/interfaces/gui_observer.h
        space_invaders/interfaces/input.h
        space_invaders/interfaces/key_observer.h
        space_invaders/interfaces/state.h
        space_invaders/states/paused_state.h
        space_invaders/states/running_state.h
        space_invaders/states/stepping_state.h
        space_invaders/states/stopped_state.h
        space_invaders/states/state_context.h

        zxspectrum_48k/audio.h
        zxspectrum_48k/cpu_io.h
        zxspectrum_48k/gui.h
        zxspectrum_48k/gui_imgui.h
        zxspectrum_48k/gui_io.h
        zxspectrum_48k/gui_sdl.h
        zxspectrum_48k/input_imgui.h
        zxspectrum_48k/input_sdl.h
        zxspectrum_48k/joystick_type.h
        zxspectrum_48k/key_request.h
        zxspectrum_48k/keyboard_pane.h
        zxspectrum_48k/memory_map_for_zxspectrum_48k.h
        zxspectrum_48k/zxspectrum_48k.h
        zxspectrum_48k/zxspectrum_48k_print_header_session.h
        zxspectrum_48k/zxspectrum_48k_session.h
        zxspectrum_48k/settings.h
        zxspectrum_48k/usage.h
        zxspectrum_48k/interfaces/gui_observer.h
        zxspectrum_48k/interfaces/input.h
        zxspectrum_48k/interfaces/key_observer.h
        zxspectrum_48k/interfaces/state.h
        zxspectrum_48k/formats/z80_format.h
        zxspectrum_48k/states/paused_state.h
        zxspectrum_48k/states/running_state.h
        zxspectrum_48k/states/stepping_state.h
        zxspectrum_48k/states/stopped_state.h
        zxspectrum_48k/states/state_context.h
        )

add_library(Applications STATIC ${SOURCES_APPLICATIONS_H} ${SOURCES_APPLICATIONS_CPP})
target_link_libraries(Applications PRIVATE
        Glad SDL2::Main SDL2::Image ImGui Doctest
        Crosscutting I8080 LR35902 LMC NamcoWsg3 Z80
        )
target_include_directories(Applications PUBLIC ../)

if (iwyu_path)
    set(iwyu_path_and_options ${iwyu_path} -Xiwyu --mapping_file=${PROJECT_SOURCE_DIR}/iwyu.imp)
    set_property(TARGET Applications PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path_and_options})
endif ()
