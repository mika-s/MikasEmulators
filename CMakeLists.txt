# ------------------------ Front matter

cmake_minimum_required(VERSION 3.17)

project(emulator LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# ------------------------ Extra packages to be found

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
#find_package(SDL2_mixer REQUIRED)

# ------------------------ Main binary

include(${PROJECT_SOURCE_DIR}/src/8080/SourceFiles.cmake)

add_executable(${PROJECT_NAME} ${SOURCES_8080_CPP} ${SOURCES_8080_H})
#target_link_libraries(${PROJECT_NAME} SDL2::Main SDL2::Image SDL2_mixer)
target_link_libraries(${PROJECT_NAME} SDL2::Main SDL2::Image)
target_include_directories(${PROJECT_NAME} PRIVATE
        src/8080
        src/8080/instructions
        src/8080/interfaces
        src/crosscutting
        )
target_compile_options(${PROJECT_NAME} PRIVATE -Werror -Wpedantic -Wall -Wextra)

# ------------------------ Test binary

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(${PROJECT_SOURCE_DIR}/src/8080/SourceFilesTest.cmake)

add_executable(${PROJECT_NAME}test ${TEST_SOURCES_8080_CPP} ${TEST_SOURCES_8080_H})
target_link_libraries(${PROJECT_NAME}test gtest_main)
target_include_directories(${PROJECT_NAME}test PRIVATE
        src/8080
        src/8080/instructions
        src/8080/interfaces
        src/crosscutting
        )

include(GoogleTest)

gtest_discover_tests(${PROJECT_NAME}test)
add_test(NAME ${PROJECT_NAME}test COMMAND ${PROJECT_NAME}test)
enable_testing(true)

# ------------------------ Copy files to working directory

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/roms/8080/invaders.e")
    configure_file(roms/8080/invaders.e invaders.e COPYONLY)
endif()
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/roms/8080/invaders.f")
    configure_file(roms/8080/invaders.f invaders.f COPYONLY)
endif()
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/roms/8080/invaders.g")
    configure_file(roms/8080/invaders.g invaders.g COPYONLY)
endif()
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/roms/8080/invaders.h")
    configure_file(roms/8080/invaders.h invaders.h COPYONLY)
endif()

configure_file(roms/8080/TST8080.COM TST8080.COM COPYONLY)
configure_file(roms/8080/8080PRE.COM 8080PRE.COM COPYONLY)
configure_file(roms/8080/8080EXM.COM 8080EXM.COM COPYONLY)
configure_file(roms/8080/CPUTEST.COM CPUTEST.COM COPYONLY)
