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
find_package(fmt)

include(${PROJECT_SOURCE_DIR}/libs/doctest/SourceFiles.cmake)
include(${PROJECT_SOURCE_DIR}/libs/glad/SourceFiles.cmake)
include(${PROJECT_SOURCE_DIR}/libs/imgui/SourceFiles.cmake)

# ------------------------ Main binary

include(${PROJECT_SOURCE_DIR}/src/8080/SourceFiles.cmake)
include(${PROJECT_SOURCE_DIR}/src/z80/SourceFiles.cmake)
include(${PROJECT_SOURCE_DIR}/src/crosscutting/SourceFiles.cmake)

add_executable(${PROJECT_NAME}
        ${SOURCES_DOCTEST} ${SOURCES_GLAD} ${SOURCES_IMGUI}
        ${SOURCES_CROSSCUTTING_CPP} ${SOURCES_CROSSCUTTING_H}
        ${SOURCES_8080_CPP} ${SOURCES_8080_H}
        ${SOURCES_Z80_CPP} ${SOURCES_Z80_H}
        )
target_link_libraries(${PROJECT_NAME} SDL2::Main SDL2::Image fmt::fmt)
target_include_directories(${PROJECT_NAME} PRIVATE
        libs/doctest
        libs/glad/include
        libs/imgui
        src
        )
target_compile_options(${PROJECT_NAME} PRIVATE -Werror -Wall -Wextra -O3)

# ------------------------ Copy files to working directory

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/roms DESTINATION ${CMAKE_BINARY_DIR})
