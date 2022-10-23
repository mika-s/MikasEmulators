# Emulators

This is my collection of emulators, written in C++.

The project is licensed under [The MIT License](LICENSE.md), with the test roms and libraries as exceptions. They have
their own licenses. See the source code of each test rom or in the library folders to see their license or copyright
notice.

## Supported chips, applications and games

- [Intel 8080](documentation/chips/8080.md)
    - [Space Invaders](documentation/applications/space_invaders.md)
- [Z80](documentation/chips/z80.md) (WIP)
    - [Pacman](documentation/applications/pacman.md) (WIP)

## Dependencies

- [SDL2](https://www.libsdl.org/) (and SDL2_image)
- [Dear Imgui](https://github.com/ocornut/imgui) (included)
- [OpenGL](https://www.opengl.org/)
- [cmake](https://cmake.org/) 3.17 or later
- [doctest](https://github.com/doctest/doctest) (included)
- [fmt](https://fmt.dev/)

## Building

CMake, fmt, OpenGL and SDL2 have to be installed before building.

Installation of dependencies:

- Arch: `sudo pacman -S cmake sdl2 sdl2_image fmt`
- Ubuntu: `sudo apt install cmake libsdl2-dev libsdl2-image-dev libfmt-dev`

The emulator can be built in debug mode like this:

```sh
mkdir cmake-build-debug
cmake -G "CodeBlocks - Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -B cmake-build-debug/
cmake --build cmake-build-debug/
```

and in release mode like this:

```sh
mkdir cmake-build-release
cmake -G "CodeBlocks - Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -B cmake-build-release/
cmake --build cmake-build-release/
```
