# ------------------------ Front matter

cmake_minimum_required(VERSION 3.17)

project(emulator LANGUAGES CXX C)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# ------------------------ Extra packages to be found

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
#find_package(SDL2_mixer REQUIRED)
find_package(fmt)

include(${PROJECT_SOURCE_DIR}/libs/doctest/SourceFiles.cmake)
include(${PROJECT_SOURCE_DIR}/libs/glad/SourceFiles.cmake)
include(${PROJECT_SOURCE_DIR}/libs/imgui/SourceFiles.cmake)

# ------------------------ Main binary

include(${PROJECT_SOURCE_DIR}/src/8080/SourceFiles.cmake)

add_executable(${PROJECT_NAME} ${SOURCES_8080_CPP} ${SOURCES_8080_H} ${SOURCES_DOCTEST} ${SOURCES_GLAD} ${SOURCES_IMGUI})
#target_link_libraries(${PROJECT_NAME} SDL2::Main SDL2::Image SDL2_mixer)
target_link_libraries(${PROJECT_NAME} SDL2::Main SDL2::Image fmt::fmt)
target_include_directories(${PROJECT_NAME} PRIVATE
        libs/doctest
        libs/glad/include
        libs/imgui
        src
        )
target_compile_options(${PROJECT_NAME} PRIVATE -Werror -Wall -Wextra)

# ------------------------ Copy files to working directory

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/roms/8080/invaders.e")
    configure_file(roms/8080/invaders.e invaders.e COPYONLY)
endif ()
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/roms/8080/invaders.f")
    configure_file(roms/8080/invaders.f invaders.f COPYONLY)
endif ()
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/roms/8080/invaders.g")
    configure_file(roms/8080/invaders.g invaders.g COPYONLY)
endif ()
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/roms/8080/invaders.h")
    configure_file(roms/8080/invaders.h invaders.h COPYONLY)
endif ()

configure_file(roms/8080/TST8080.COM TST8080.COM COPYONLY)
configure_file(roms/8080/8080PRE.COM 8080PRE.COM COPYONLY)
configure_file(roms/8080/8080EXM.COM 8080EXM.COM COPYONLY)
configure_file(roms/8080/CPUTEST.COM CPUTEST.COM COPYONLY)
