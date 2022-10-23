set(SOURCES_APPLICATIONS_CPP
        src/applications/frontend.cpp
        src/applications/options.cpp

        src/applications/cpm_8080/cpm_application.cpp
        src/applications/cpm_8080/cpm_application_session.cpp

        src/applications/cpm_z80/cpm_application.cpp
        src/applications/cpm_z80/cpm_application_session.cpp

        src/applications/pacman/audio.cpp
        src/applications/pacman/gui.cpp
        src/applications/pacman/gui_imgui.cpp
        src/applications/pacman/gui_sdl.cpp
        src/applications/pacman/input_imgui.cpp
        src/applications/pacman/input_sdl.cpp
        src/applications/pacman/memory_mapped_io_for_pacman.cpp
        src/applications/pacman/pacman.cpp
        src/applications/pacman/pacman_session.cpp

        src/applications/space_invaders/audio.cpp
        src/applications/space_invaders/cpu_io.cpp
        src/applications/space_invaders/gui_imgui.cpp
        src/applications/space_invaders/gui_sdl.cpp
        src/applications/space_invaders/input_imgui.cpp
        src/applications/space_invaders/input_sdl.cpp
        src/applications/space_invaders/space_invaders.cpp
        src/applications/space_invaders/space_invaders_session.cpp
        )

set(SOURCES_APPLICATIONS_H
        src/applications/frontend.h
        src/applications/options.h

        src/applications/cpm_8080/cpm_application.h
        src/applications/cpm_8080/cpm_application_session.h

        src/applications/cpm_z80/cpm_application.h
        src/applications/cpm_z80/cpm_application_session.h

        src/applications/pacman/audio.h
        src/applications/pacman/gui.h
        src/applications/pacman/gui_imgui.h
        src/applications/pacman/gui_sdl.h
        src/applications/pacman/input_imgui.h
        src/applications/pacman/input_sdl.h
        src/applications/pacman/io_request.h
        src/applications/pacman/memory_mapped_io_for_pacman.h
        src/applications/pacman/pacman.h
        src/applications/pacman/pacman_session.h
        src/applications/pacman/settings.h
        src/applications/pacman/interfaces/input.h
        src/applications/pacman/interfaces/io_observer.h

        src/applications/space_invaders/audio.h
        src/applications/space_invaders/cpu_io.h
        src/applications/space_invaders/gui.h
        src/applications/space_invaders/gui_imgui.h
        src/applications/space_invaders/gui_sdl.h
        src/applications/space_invaders/input_imgui.h
        src/applications/space_invaders/input_sdl.h
        src/applications/space_invaders/io_request.h
        src/applications/space_invaders/sounds.h
        src/applications/space_invaders/space_invaders.h
        src/applications/space_invaders/space_invaders_session.h
        src/applications/space_invaders/settings.h
        src/applications/space_invaders/interfaces/input.h
        src/applications/space_invaders/interfaces/io_observer.h
        )
