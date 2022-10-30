set(SOURCES_IMGUI
        imconfig.h
        imgui.cpp
        imgui.h
        imgui_demo.cpp
        imgui_draw.cpp
        imgui_impl_opengl3.cpp
        imgui_impl_opengl3.h
        imgui_impl_opengl3_loader.h
        imgui_impl_sdl.cpp
        imgui_impl_sdl.h
        imgui_internal.h
        imgui_memory_editor.h
        imgui_tables.cpp
        imgui_widgets.cpp
        imstb_rectpack.h
        imstb_textedit.h
        imstb_truetype.h
        )

add_library(ImGui STATIC ${SOURCES_IMGUI})
target_link_libraries(ImGui SDL2::Main SDL2::Image)
target_include_directories(ImGui PUBLIC ./)
