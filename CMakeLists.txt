# ------------------------ Front matter

cmake_minimum_required(VERSION 3.21)

project(emulator LANGUAGES CXX C)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# ------------------------ Third party

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(fmt REQUIRED)
find_program(iwyu_path NAMES include-what-you-use iwyu)

add_subdirectory(${PROJECT_SOURCE_DIR}/libs/doctest)
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/glad/)
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/imgui/)

# ------------------------ Main binary

add_subdirectory(${PROJECT_SOURCE_DIR}/src/crosscutting)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/chips/8080)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/chips/namco_wsg3)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/chips/z80)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/applications)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE
        SDL2::Main
        SDL2::Image
        Glad
        ImGui
        Doctest
        fmt::fmt
        Crosscutting
        I8080
        NamcoWsg3
        Z80
        Applications
        )

if(iwyu_path)
  message(STATUS "Found include-what-you-use. Applying it to the project.")
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
endif()

# ------------------------ Copy files to working directory

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/roms DESTINATION ${CMAKE_BINARY_DIR})
