# Emulators

This is my collection of emulators, written in C++. As of now, it only emulates Intel 8080, with the game Space Invaders.

## Table of contents

* [Dependencies](#dependencies)
* [Building](#building)
* [Emulated CPUs](#emulated-cpus)
  * [Intel 8080](#intel-8080)
    * [Space Invaders](#space-invaders)
    * [Inspiration](#inspiration)
    * [Todo](#todo)
  * [Z80](#z80)
    * [Pacman](#pacman)
    * [Inspiration](#inspiration-1)
    * [Todo](#todo-1)


## Dependencies

- [SDL2](https://www.libsdl.org/) (and SDL2_image)
- [Dear Imgui](https://github.com/ocornut/imgui) (included)
- [OpenGL](https://www.opengl.org/)
- [cmake](https://cmake.org/) 3.17 or better
- [doctest](https://github.com/doctest/doctest)
- [fmt](https://fmt.dev/)

## Building

CMake, fmt, OpenGL and SDL2 have to be installed before building.

```sh
mkdir cmake-build-debug
cmake -G "CodeBlocks - Unix Makefiles" -B cmake-build-debug/
cmake --build cmake-build-debug/
```

## Emulated CPUs

### Intel 8080

Passes the following test binaries:

- `CPUTEST.COM`:

```
DIAGNOSTICS II V1.2 - CPU TEST
COPYRIGHT (C) 1981 - SUPERSOFT ASSOCIATES

ABCDEFGHIJKLMNOPQRSTUVWXYZ
CPU IS 8080/8085
BEGIN TIMING TEST
END TIMING TEST
CPU TESTS OK
```

- `TST8080.COM`:

```
MICROCOSM ASSOCIATES 8080/8085 CPU DIAGNOSTIC
 VERSION 1.0  (C) 1980

 CPU IS OPERATIONAL
```

- `8080PRE.COM`:

```
8080 Preliminary tests complete
```

- `8080EXM.COM`:

```
8080 instruction exerciser
dad <b,d,h,sp>................  PASS! crc is:14474ba6
aluop nn......................  PASS! crc is:9e922f9e
aluop <b,c,d,e,h,l,m,a>.......  PASS! crc is:cf762c86
<daa,cma,stc,cmc>.............  PASS! crc is:bb3f030c
<inr,dcr> a...................  PASS! crc is:adb6460e
<inr,dcr> b...................  PASS! crc is:83ed1345
<inx,dcx> b...................  PASS! crc is:f79287cd
<inr,dcr> c...................  PASS! crc is:e5f6721b
<inr,dcr> d...................  PASS! crc is:15b5579a
<inx,dcx> d...................  PASS! crc is:7f4e2501
<inr,dcr> e...................  PASS! crc is:cf2ab396
<inr,dcr> h...................  PASS! crc is:12b2952c
<inx,dcx> h...................  PASS! crc is:9f2b23c0
<inr,dcr> l...................  PASS! crc is:ff57d356
<inr,dcr> m...................  PASS! crc is:92e963bd
<inx,dcx> sp..................  PASS! crc is:d5702fab
lhld nnnn.....................  PASS! crc is:a9c3d5cb    
shld nnnn.....................  PASS! crc is:e8864f26
lxi <b,d,h,sp>,nnnn...........  PASS! crc is:fcf46e12
ldax <b,d>....................  PASS! crc is:2b821d5f
mvi <b,c,d,e,h,l,m,a>,nn......  PASS! crc is:eaa72044
mov <bcdehla>,<bcdehla>.......  PASS! crc is:10b58cee
sta nnnn / lda nnnn...........  PASS! crc is:ed57af72
<rlc,rrc,ral,rar>.............  PASS! crc is:e0d89235
stax <b,d>....................  PASS! crc is:2b0471e9
Tests complete
```

The CP/M test binaries can be run like this:

```sh
cd cmake-build-debug/
./emulator 8080 run CPUTEST
./emulator 8080 run TST8080
./emulator 8080 run 8080PRE
./emulator 8080 run 8080EXM
```

Unlike Space Invaders, the test binaries are provided.

#### Space Invaders

The emulator can also play Space Invaders. The rom files are not provided. To run Space Invaders, run the following after building:

```sh
cd cmake-build-debug/
./emulator 8080 run space_invaders
```

The four rom files (invaders.e, invaders.f, invaders.g and invaders.h) have to be in the same directory as the emulator binary.

It is possible to provide arguments that sets the DIP switches. The flag is `-d`, and it has the following options:

- `n=3`, `n=4`, `n=5` or `n=6` sets number of lives. The default value, if unset, is 3 lives.
- `b=1000` or `b=1500` sets bonus life at a score of 1000 or 1500. The default value, if unset, is bonus live at 1500.
- `c=on` or `c=off` sets coin on or off. Doesn't seem to do anything.

An example:

```sh
./emulator 8080 run space_invaders -d n=5 -d b=1500
```

sets number of lives to 5 and bonus life at a score of 1500.

It is also possible to switch between the ordinary GUI (plain SDL) and the debugging GUI (based on Dear Imgui), using
the `-g` flag:

- `ordinary` starts the ordinary GUI. The default value, if unset.
- `debugging` starts the debugging GUI.

An example:

```sh
./emulator 8080 run space_invaders -g debugging
```

The keymap is:

| Key     | Description   |
| ------- | ------------- |
| C       | Insert coin   |
| T       | Tilt          |
| Enter   | P1 start      |
| A       | P1 left       |
| D       | P1 right      |
| W       | P1 shoot      |
| R Shift | P2 start      |
| Left    | P2 left       |
| Right   | P2 right      |
| Up      | P2 shoot      |
| Pause   | Pause/unpause |
| M       | Mute/unmute   |

Extra keys while debugging:

| Key     | Description        |
| ------- | ------------------ |
| F7      | Step instruction   |
| F8      | Step cycle         |
| F9      | Continue execution |

Screenshots:

![Plain Space Invaders](assets/images/8080_SI.png "Plain Space Invaders")

![Space Invaders with debugger](assets/images/8080_SI_debugger.png "Space Invaders with debugger")

#### Inspiration

I looked at the following emulators while developing my 8080 emulator:

- [herrecito's Space Invaders / Intel 8080 emulator](https://github.com/herrecito/invaders): Mainly for the timing loop.
- [Superzazu's Space Invaders emulator](https://github.com/superzazu/invaders): Mainly the drawing to screen.
- [Dromaius, a C++ gameboy emulator / debugger](https://github.com/ThomasRinsma/dromaius): Dear Imgui implementation.

The [Computer Archeology](http://computerarcheology.com/Arcade/SpaceInvaders/) page on Space Invaders was also used to
look up how the game is supposed to behave.

#### Todo

- [ ] Fix the CMake file to use better best practices
- [ ] Better argument handling
- [ ] Refactor to "sessions" for Space Invaders and the 8080 cpu


### Z80

#### Pacman

#### Inspiration

#### Todo
