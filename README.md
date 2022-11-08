# CHIP-8 emulator
An interpreter implementation for the fictional CHIP-8 machine language. The emulator is written in C and uses SDL2 as graphics library. Make sure to adjust the `Makefile` with your own include and linking paths. 

Currently, the emulator passes [corax89's chip8-test-rom](https://github.com/corax89/chip8-test-rom). Most instructions are implemented. However, audio does not work yet.  

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
