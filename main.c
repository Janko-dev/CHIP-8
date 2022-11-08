#include "chip8.h"

int main(int argc, char** argv){
    (void)argc; (void)argv; 

    CHIP8* chip = create_CHIP8();
    load_ROM(chip, "roms/chip8-test-suite.ch8");
    // load_ROM(chip, "resources/test_opcode.ch8");

    while (chip->is_running) {

        handle_events(chip);
        interpret(chip);

        if (chip->draw_flag){
            render(chip);
        }

        // decrease timers
        chip->delay_timer = chip->delay_timer > 0 ? chip->delay_timer - 1 : 0;
        if (chip->sound_timer > 0){
            chip->sound_timer--;
        } else {
            SDL_PauseAudio(1); // stop playing sound
        } 

        SDL_Delay(1000/60);
    }

    destroy_CHIP8(chip);
    return 0;
}