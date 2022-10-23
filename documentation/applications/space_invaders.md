# Space Invaders

The rom files are not provided. To run Space Invaders, run the following after building:

```sh
cd cmake-build-debug/
./emulator run space_invaders
```

The four rom files (invaders.e, invaders.f, invaders.g and invaders.h) have to be in the directory
`roms/8080/space_invaders/`, using the emulator binary folder as root folder.

It is possible to provide arguments that sets the DIP switches. The flag is `-d`, and it has the following options:

- `n=3`, `n=4`, `n=5` or `n=6` sets number of lives. The default value, if unset, is 3 lives.
- `b=1000` or `b=1500` sets bonus life at a score of 1000 or 1500. The default value, if unset, is bonus life at 1500.
- `c=on` or `c=off` sets coin on or off. Doesn't seem to do anything.

An example:

```sh
./emulator run space_invaders -d n=5 -d b=1500
```

sets number of lives to 5 and bonus life at a score of 1500.

It is also possible to switch between the ordinary GUI (plain SDL) and the debugging GUI (based on Dear Imgui), using
the `-g` flag:

- `ordinary` starts the ordinary GUI. The default value, if unset.
- `debugging` starts the debugging GUI.

An example:

```sh
./emulator run space_invaders -g debugging
```

The keymap is:

<table>
<tr>
<th>Gameplay</th><th>Debugging</th>
</tr>
<tr>
<td>

| Key                | Description   |
|--------------------|---------------|
| <kbd>C</kbd>       | Insert coin   |
| <kbd>T</kbd>       | Tilt          |
| <kbd>Enter</kbd>   | P1 start      |
| <kbd>A</kbd>       | P1 left       |
| <kbd>D</kbd>       | P1 right      |
| <kbd>W</kbd>       | P1 shoot      |
| <kbd>R Shift</kbd> | P2 start      |
| <kbd>←</kbd>       | P2 left       |
| <kbd>→</kbd>       | P2 right      |
| <kbd>↑</kbd>       | P2 shoot      |
| <kbd>Pause</kbd>   | Pause/unpause |
| <kbd>M</kbd>       | Mute/unmute   |

</td>
<td>

| Key           | Description        |
|---------------|--------------------|
| <kbd>F7</kbd> | Step instruction   |
| <kbd>F8</kbd> | Step cycle         |
| <kbd>F9</kbd> | Continue execution |

</td>
</tr>
</table>

Screenshots:

|                                                                                 |                                                                                                          |
|:-------------------------------------------------------------------------------:|:--------------------------------------------------------------------------------------------------------:|
| ![Plain Space Invaders](../../assets/images/8080_SI.png "Plain Space Invaders") | ![Space Invaders with debugger](../../assets/images/8080_SI_debugger.png "Space Invaders with debugger") | 
|                             *Plain Space Invaders*                              |                                   *Space Invaders in the debugger GUI*                                   |
