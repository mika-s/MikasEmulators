set(SOURCES_APPLICATIONS_CPP
        frontend.cpp
        options.cpp

        cpm_8080/cpm_application.cpp
        cpm_8080/cpm_application_session.cpp

        cpm_z80/cpm_application.cpp
        cpm_z80/cpm_application_session.cpp

        pacman/audio.cpp
        pacman/gui.cpp
        pacman/gui_imgui.cpp
        pacman/gui_sdl.cpp
        pacman/input_imgui.cpp
        pacman/input_sdl.cpp
        pacman/memory_mapped_io_for_pacman.cpp
        pacman/pacman.cpp
        pacman/pacman_session.cpp

        space_invaders/audio.cpp
        space_invaders/cpu_io.cpp
        space_invaders/gui_imgui.cpp
        space_invaders/gui_sdl.cpp
        space_invaders/input_imgui.cpp
        space_invaders/input_sdl.cpp
        space_invaders/memory_map_for_space_invaders.cpp
        space_invaders/space_invaders.cpp
        space_invaders/space_invaders_session.cpp
        )

set(SOURCES_APPLICATIONS_H
        frontend.h
        options.h

        cpm_8080/cpm_application.h
        cpm_8080/cpm_application_session.h

        cpm_z80/cpm_application.h
        cpm_z80/cpm_application_session.h

        pacman/audio.h
        pacman/gui.h
        pacman/gui_imgui.h
        pacman/gui_sdl.h
        pacman/input_imgui.h
        pacman/input_sdl.h
        pacman/io_request.h
        pacman/memory_mapped_io_for_pacman.h
        pacman/pacman.h
        pacman/pacman_session.h
        pacman/settings.h
        pacman/interfaces/input.h
        pacman/interfaces/io_observer.h

        space_invaders/audio.h
        space_invaders/cpu_io.h
        space_invaders/gui.h
        space_invaders/gui_imgui.h
        space_invaders/gui_sdl.h
        space_invaders/input_imgui.h
        space_invaders/input_sdl.h
        space_invaders/io_request.h
        space_invaders/memory_map_for_space_invaders.h
        space_invaders/sounds.h
        space_invaders/space_invaders.h
        space_invaders/space_invaders_session.h
        space_invaders/settings.h
        space_invaders/interfaces/input.h
        space_invaders/interfaces/io_observer.h
        )

add_library(Applications STATIC ${SOURCES_APPLICATIONS_H} ${SOURCES_APPLICATIONS_CPP})
target_link_libraries(Applications PRIVATE Glad SDL2::Main SDL2::Image ImGui Doctest Crosscutting I8080 NamcoWsg3 Z80)
target_include_directories(Applications PUBLIC ../)
