#include "chip8.h"

int main(int argc, char** argv){
    (void)argc; (void)argv; 

    CHIP8* chip = create_CHIP8();
    load_ROM(chip, "resources/c8_test.c8");
    // load_ROM(chip, "resources/test_opcode.ch8");
    // load_ROM(chip, "resources/br8kout.ch8");
    // load_ROM(chip, "resources/tetris.ch8");

    // chip->mem[512] = 0x65; // v5 := ff
    // chip->mem[513] = 0xff;

    // chip->mem[514] = 0xF5; // sound v5 -> ff
    // chip->mem[515] = 0x18;

    // chip->mem[516] = 0x12; // loop
    // chip->mem[517] = 0x04;

    while (chip->is_running) {
        // Uint64 start = SDL_GetPerformanceCounter();

        handle_events(chip);
        interpret(chip);
        if (chip->draw_flag){
            render(chip);
        }
        // Uint64 end = SDL_GetPerformanceCounter();
        // float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
        
        // Cap to 60 FPS
        // SDL_Delay(floor(16.666f - elapsedMS));
        
        SDL_Delay(10);
    }

    destroy_CHIP8(chip);
    return 0;
}