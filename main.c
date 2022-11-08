#include "chip8.h"

int main(int argc, char** argv){
    (void)argc; (void)argv; 

    CHIP8* chip = create_CHIP8();
    // load_ROM(chip, "resources/c8_test.c8");
    load_ROM(chip, "resources/test_opcode.ch8");

    // chip->mem[512] = 0x65; // v5 := 9
    // chip->mem[513] = 0x09;

    // chip->mem[514] = 0x61; // v1 := 5
    // chip->mem[515] = 0x05;
    
    // chip->mem[516] = 0x62; // v2 := 5
    // chip->mem[517] = 0x05;

    // chip->mem[518] = 0xF5; // I := sprite data of "9"
    // chip->mem[519] = 0x29;

    // chip->mem[520] = 0xD1; // draw at 5, 5 sprite with data from I
    // chip->mem[521] = 0x25;

    // chip->mem[522] = 0x12; // loop
    // chip->mem[523] = 0x0A;

    while (chip->is_running) {
        // Uint64 start = SDL_GetPerformanceCounter();

        handle_events(chip);
        interpret(chip);
        render(chip);
        
        // Uint64 end = SDL_GetPerformanceCounter();
        // float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
        
        // Cap to 60 FPS
        // SDL_Delay(floor(16.666f - elapsedMS));
        
        SDL_Delay(10);
    }

    destroy_CHIP8(chip);
    return 0;
}