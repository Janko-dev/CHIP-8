# CHIP-8 emulator
An interpreter implementation for the fictional CHIP-8 assembly language. The emulator is written in C and uses [SDL2](https://www.libsdl.org/) as graphics library. Make sure to adjust the `Makefile` with your own include and linking paths to the SDL2 library if you are running on Windows. On Linux, make you do not have to touch the `Makefile`. However, make sure SDL2 is installed with the following line

```
sudo apt-get install libsdl2-dev
```

Currently, the emulator passes [corax89's chip8-test-rom](https://github.com/corax89/chip8-test-rom). Most instructions are implemented. However, audio is not implemented yet.

## Keyboard layout
The keyboard mapping is the following. 
```
  keyboard                  layout
[1][2][3][4]   ------>   [1][2][3][c]
[q][w][e][r]   maps to   [4][5][6][d]
[a][s][d][f]   ------>   [7][8][9][e]
[z][x][c][v]             [a][0][b][f]
```

## Quick start
```
$ git clone https://github.com/Janko-dev/CHIP-8
$ cd CHIP-8
$ make 
$ ./chip8.exe
```

## References:
- https://github.com/corax89/chip8-test-rom
- https://johnearnest.github.io/chip8Archive/
- https://github.com/mattmikolay/chip-8/wiki/Mastering-CHIP%E2%80%908#resources
- https://github.com/Timendus/chip8-test-suite#flags-test 
