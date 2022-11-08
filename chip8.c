#include "chip8.h"

uint8_t fontset[80] = { 
    0xF0, 0x90, 0x90, 0x90, 0xF0, //0
    0x20, 0x60, 0x20, 0x20, 0x70, //1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
    0x90, 0x90, 0xF0, 0x10, 0x10, //4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
    0xF0, 0x10, 0x20, 0x40, 0x40, //7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
    0xF0, 0x90, 0xF0, 0x90, 0x90, //A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
    0xF0, 0x80, 0x80, 0x80, 0xF0, //C
    0xE0, 0x90, 0x90, 0x90, 0xE0, //D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
    0xF0, 0x80, 0xF0, 0x80, 0x80  //F
};

void audio_callback(void *user_data, Uint8 *raw_buffer, int bytes)
{
    Sint16 *buffer = (Sint16*)raw_buffer;
    int length = bytes / 2; // 2 bytes per sample for AUDIO_S16SYS
    int sample_nr = (*(int*)user_data);

    for(int i = 0; i < length; i++, sample_nr++)
    {
        double time = (double)sample_nr / (double)SAMPLE_RATE;
        buffer[i] = (Sint16)(AMPLITUDE * sin(2.0f * M_PI * 441.0f * time)); // render 441 HZ sine wave
    }
}

CHIP8* create_CHIP8(){
    CHIP8* chip = (CHIP8*)calloc(1, sizeof(CHIP8));
    if (chip == NULL) {
        fprintf(stderr, "Cannot allocate CHIP-8 struct");
        exit(1);
    }

    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_AUDIO) != 0){
        fprintf(stderr, "Unable to init subsystems: %s\n", SDL_GetError());
        exit(1);
    }

    chip->state.window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    if (chip->state.window == NULL){
        fprintf(stderr, "Unable to create window: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    chip->state.renderer = SDL_CreateRenderer(chip->state.window, -1, 0);
    if (chip->state.renderer == NULL){
        fprintf(stderr, "Unable to create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(chip->state.window);
        SDL_Quit();
        exit(1);
    }

    // int sample_nr = 0;

    SDL_AudioSpec want, have;
    SDL_memset(&want, 0, sizeof(want));
    want.freq = SAMPLE_RATE; // number of samples per second
    want.format = AUDIO_S16SYS; // sample type (here: signed short i.e. 16 bit)
    want.channels = 1; // only one channel
    want.samples = 2048; // buffer-size
    want.callback = audio_callback; // function SDL calls periodically to refill the buffer
    want.userdata = NULL; //&sample_nr; // counter, keeping track of current sample number

    if(SDL_OpenAudio(&want, &have) != 0) SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "Failed to open audio: %s", SDL_GetError());
    if(want.format != have.format) SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "Failed to get the desired AudioSpec");

    srand(time(NULL));
    chip->is_running = true;
    chip->pc = 0x200; 
    return chip;
}

void destroy_CHIP8(CHIP8* chip){
    SDL_CloseAudio();
    SDL_DestroyRenderer(chip->state.renderer);
    SDL_DestroyWindow(chip->state.window);
    SDL_Quit();
    free(chip);
}

void load_ROM(CHIP8* chip, const char* file_path){
    FILE* f = fopen(file_path, "rb");
    fseek(f, 0L, SEEK_END);
    size_t size = ftell(f);
    fseek(f, 0L, SEEK_SET);
    fread(chip->mem + 0x200, sizeof(uint8_t), size, f);
    fclose(f);
    memcpy(chip->mem, fontset, 80);
}

void handle_events(CHIP8* chip){
    SDL_Event e;
    while(SDL_PollEvent(&e)){
        switch(e.type){
            case SDL_QUIT:
                chip->is_running = false;
                break;
            case SDL_KEYDOWN:{
                switch (e.key.keysym.sym){
                    case SDLK_1: chip->keys[0x1] = 1; break;
                    case SDLK_2: chip->keys[0x2] = 1; break;
                    case SDLK_3: chip->keys[0x3] = 1; break;
                    case SDLK_4: chip->keys[0xc] = 1; break;
                    case SDLK_q: chip->keys[0x4] = 1; break;
                    case SDLK_w: chip->keys[0x5] = 1; break;
                    case SDLK_e: chip->keys[0x6] = 1; break;
                    case SDLK_r: chip->keys[0xd] = 1; break;
                    case SDLK_a: chip->keys[0x7] = 1; break;
                    case SDLK_s: chip->keys[0x8] = 1; break;
                    case SDLK_d: chip->keys[0x9] = 1; break;
                    case SDLK_f: chip->keys[0xe] = 1; break;
                    case SDLK_z: chip->keys[0xa] = 1; break;
                    case SDLK_x: chip->keys[0x0] = 1; break;
                    case SDLK_c: chip->keys[0xb] = 1; break;
                    case SDLK_v: chip->keys[0xf] = 1; break;
                    default: break;
                }
            } break;
            case SDL_KEYUP:{
                switch (e.key.keysym.sym){
                    case SDLK_1: chip->keys[0x1] = 0; break;
                    case SDLK_2: chip->keys[0x2] = 0; break;
                    case SDLK_3: chip->keys[0x3] = 0; break;
                    case SDLK_4: chip->keys[0xc] = 0; break;
                    case SDLK_q: chip->keys[0x4] = 0; break;
                    case SDLK_w: chip->keys[0x5] = 0; break;
                    case SDLK_e: chip->keys[0x6] = 0; break;
                    case SDLK_r: chip->keys[0xd] = 0; break;
                    case SDLK_a: chip->keys[0x7] = 0; break;
                    case SDLK_s: chip->keys[0x8] = 0; break;
                    case SDLK_d: chip->keys[0x9] = 0; break;
                    case SDLK_f: chip->keys[0xe] = 0; break;
                    case SDLK_z: chip->keys[0xa] = 0; break;
                    case SDLK_x: chip->keys[0x0] = 0; break;
                    case SDLK_c: chip->keys[0xb] = 0; break;
                    case SDLK_v: chip->keys[0xf] = 0; break;
                    default: break;
                }
            } break;
            default: break;
        }
    }
}

void render(CHIP8* chip){
    
    chip->draw_flag = false;
    SDL_Texture* tex = SDL_CreateTexture(chip->state.renderer, 
        SDL_PIXELFORMAT_RGBA8888, 
        SDL_TEXTUREACCESS_TARGET,
        WIDTH, HEIGHT);

    SDL_SetRenderTarget(chip->state.renderer, tex);
    SDL_SetRenderDrawColor(chip->state.renderer, 0, 0, 0, 0);
    SDL_RenderClear(chip->state.renderer);
    for (int i = 0; i < PIXEL_HEIGHT*PIXEL_WIDTH; i++){
        SDL_Rect r = {
            .x=(i%PIXEL_WIDTH)*10,
            .y=(i/PIXEL_WIDTH)*10,
            .w=10, .h=10
        };
        if (chip->pixels[i]){
            SDL_SetRenderDrawColor(chip->state.renderer, 255, 255, 255, 255);
        } else {
            SDL_SetRenderDrawColor(chip->state.renderer, 0, 0, 0, 0);
        }
        SDL_RenderFillRect(chip->state.renderer, &r);
    }
    SDL_SetRenderTarget(chip->state.renderer, NULL);
    SDL_RenderCopy(chip->state.renderer, tex, NULL, NULL);
    SDL_RenderPresent(chip->state.renderer);
    SDL_DestroyTexture(tex);
}

void interpret(CHIP8* chip){
    
    // printf("Code (pc: 0x%04x): 0x%02x%02x\n", chip->pc, chip->mem[chip->pc], chip->mem[chip->pc+1]);

    uint8_t* code = chip->mem + chip->pc;
    chip->pc+=2;

    switch (HIGH(*code)){
        case 0x60: // 6XNN	Store number NN in register VX
            chip->reg[LOW(*code)] = *(code+1);
            break;
        case 0x80: { 
            uint8_t arg = *(code+1);
            uint8_t* vx = &chip->reg[LOW(*code)];
            uint8_t* vy = &chip->reg[HIGH(arg) >> 4];
            switch (LOW(arg)){
                case 0x00: // 8XY0	Store the value of register VY in register VX
                    *vx = *vy;
                    break;
                case 0x04:{ // 8XY4  Add the value of register VY to register VX. Set VF to 01 if a carry occurs. Set VF to 00 if a carry does not occur
                    if (*vx > UINT8_MAX - *vy) chip->reg[0xf] = 1;
                    else chip->reg[0xf] = 0;
                    *vx += *vy;
                } break;
                case 0x05:{ // 8XY5	Subtract the value of register VY from register VX. Set VF to 00 if a borrow occurs. Set VF to 01 if a borrow does not occur
                    if (*vx < *vy) chip->reg[0xf] = 0;
                    else chip->reg[0xf] = 1;
                    *vx -= *vy;
                } break;
                case 0x07:{ // 8XY7	Set register VX to the value of VY minus VX. Set VF to 00 if a borrow occurs. Set VF to 01 if a borrow does not occur
                    if (*vy < *vx) chip->reg[0xf] = 0;
                    else chip->reg[0xf] = 1;
                    *vx = *vy - *vx;
                } break;
                case 0x02: // 8XY2	Set VX to VX AND VY
                    *vx &= *vy; break;
                case 0x01: // 8XY1	Set VX to VX OR VY
                    *vx |= *vy; break;
                case 0x03: // 8XY3	Set VX to VX XOR VY
                    *vx ^= *vy; break;
                case 0x06: // 8XY6	Store the value of register VY shifted right one bit in register VX. Set register VF to the least significant bit prior to the shift
                    chip->reg[0xf] = *vy & 0x01;
                    *vx = *vy >> 1; break;
                case 0x0E: // 8XYE	Store the value of register VY shifted left one bit in register VX. Set register VF to the most significant bit prior to the shift
                    chip->reg[0xf] = (*vy & 0x80) >> 7;
                    *vx = *vy << 1; break;
                default: 
                    fprintf(stderr, "Encountered: 0x%02x%02x\n", *code, *(code+1));
                    chip->is_running = false; 
                    break;
            }
        } break;
        case 0x70: // 7XNN	Add the value NN to register VX
            chip->reg[LOW(*code)] += *(code+1); break;
        case 0xC0: // CXNN	Set VX to a random number with a mask of NN
            chip->reg[LOW(*code)] = (rand() % 0xFF) & *(code+1); break;
        case 0x10: // 1NNN	Jump to address NNN
            chip->pc = ((LOW(*code) << 8) | *(code+1)); // lower byte of opcode shifted to left by 16 bits, then added with operand.
            return; 
        case 0xB0: // BNNN	Jump to address NNN + V0
            chip->pc = ((LOW(*code) << 8) | *(code+1)) + chip->reg[0]; 
            return;
        case 0x20: // 2NNN  Execute subroutine starting at address NNN
            chip->stack[chip->sp++] = chip->pc;
            chip->pc = ((LOW(*code) << 8) | *(code+1)); 
            return; 
        case 0x00: {
            switch (*(code+1)){
                case 0xEE: // 00EE	Return from a subroutine
                    chip->sp = chip->sp != 0 ? chip->sp - 1 : 0;
                    chip->pc = chip->stack[chip->sp]; 
                    break;
                case 0xE0: // 00E0	Clear the screen
                    memset(chip->pixels, 0, PIXEL_HEIGHT * PIXEL_WIDTH);
                    break;
                default: // 0NNN	Execute machine language subroutine at address NNN; not implemented
                    fprintf(stderr, "Encountered: 0x%02x%02x\n", *code, *(code+1));
                    chip->is_running = false; 
                    break;
            }
        } break;
        case 0x30: // 3XNN	Skip the following instruction if the value of register VX equals NN
            if (chip->reg[LOW(*code)] == *(code+1)) chip->pc += 2;
            break;
        case 0x50: // 5XY0	Skip the following instruction if the value of register VX is equal to the value of register VY
            if (chip->reg[LOW(*code)] == chip->reg[HIGH(*(code+1)) >> 4]) chip->pc += 2;
            break;
        case 0x40: // 4XNN	Skip the following instruction if the value of register VX is not equal to NN
            if (chip->reg[LOW(*code)] != *(code+1)) chip->pc += 2;
            break;
        case 0x90: // 9XY0	Skip the following instruction if the value of register VX is not equal to the value of register VY
            if (chip->reg[LOW(*code)] != chip->reg[HIGH(*(code+1)) >> 4]) chip->pc += 2;
            break;
        case 0xF0: 
            switch (*(code+1)){
                case 0x15: // FX15	Set the delay timer to the value of register VX
                    chip->delay_timer = chip->reg[LOW(*code)]; break;
                case 0x07: // FX07	Store the current value of the delay timer in register VX
                    chip->reg[LOW(*code)] = chip->delay_timer; break;
                case 0x18: // FX18	Set the sound timer to the value of register VX
                    chip->sound_timer = chip->reg[LOW(*code)];
                    SDL_PauseAudio(0); // start playing sound
                    break;
                case 0x0A: {// FX0A	Wait for a keypress and store the result in register VX
                    bool pressed = false;
                    for (int i = 0; i < 16; i++){
                        if (chip->keys[i]){
                            pressed = true;
                            chip->reg[LOW(*code)] = i;
                        }
                    }
                    if (!pressed) return;
                } break;
                case 0x1E: // FX1E	Add the value stored in register VX to register I
                    chip->I += chip->reg[LOW(*code)]; break;
                case 0x29: // FX29	Set I to the memory address of the sprite data corresponding to the hexadecimal digit stored in register VX
                    chip->I = chip->reg[LOW(*code)] * 5; break; // each font character is 5 bytes long
                case 0x33: { // FX33	Store the binary-coded decimal equivalent of the value stored in register VX at addresses I, I+1, and I+2
                    uint8_t vx = chip->reg[LOW(*code)];
                    chip->mem[chip->I] = vx/100;
                    chip->mem[chip->I+1] = (vx/10)%10;
                    chip->mem[chip->I+2] = (vx%10)%10;
                } break;
                case 0x55: { // FX55	Store the values of registers V0 to VX inclusive in memory starting at address I. I is set to I + X + 1 after operation
                    for (int i = 0; i < LOW(*code) + 1; i++){
                        chip->mem[chip->I + i] = chip->reg[i];
                    }
                    chip->I += LOW(*code) + 1;
                } break;
                case 0x65: { // Fill registers V0 to VX inclusive with the values stored in memory starting at address I. I is set to I + X + 1 after operation
                    for (int i = 0; i < LOW(*code) + 1; i++){
                        chip->reg[i] = chip->mem[chip->I + i];
                    }
                    chip->I += LOW(*code) + 1;
                } break;
                default: 
                    fprintf(stderr, "Encountered: 0x%02x%02x\n", *code, *(code+1));
                    chip->is_running = false; 
                    break;
            } break;
        case 0xE0: 
            switch (*(code+1)){
                case 0x9E: // EX9E	Skip the following instruction if the key corresponding to the hex value currently stored in register VX is pressed
                    if (chip->keys[chip->reg[LOW(*code)]]) chip->pc += 2; 
                    break;
                case 0xA1: // EXA1	Skip the following instruction if the key corresponding to the hex value currently stored in register VX is not pressed
                    if (!chip->keys[chip->reg[LOW(*code)]]) chip->pc += 2; 
                    break;
                default: 
                    fprintf(stderr, "Encountered: 0x%02x%02x\n", *code, *(code+1));
                    chip->is_running = false; break;
            } break;
        case 0xA0: // ANNN	Store memory address NNN in register I
            chip->I = ((LOW(*code) << 8) | *(code+1)); break;
        case 0xD0: { // DXYN Draw a sprite at position VX, VY with N bytes of sprite data starting at the address stored in I. Set VF to 01 if any set pixels are changed to unset, and 00 otherwise
            uint8_t vx_pos = chip->reg[LOW(*code)];
            uint8_t vy_pos = chip->reg[HIGH(*(code+1)) >> 4];
            uint8_t height = LOW(*(code+1));
            uint8_t pixel_row = chip->mem[chip->I];
            chip->reg[0xF] = 0;
            for (int y = 0; y < height; y++){
                pixel_row = chip->mem[chip->I + y];
                for (int x = 0; x < 8; x++){
                    if ((pixel_row & (0x80 >> x)) != 0){
                        if (chip->pixels[vx_pos + x + PIXEL_WIDTH * (vy_pos + y)] == 1)
                            chip->reg[0xF] = 1;

                        chip->pixels[vx_pos + x + PIXEL_WIDTH * (vy_pos + y)] ^= 1;                        
                    }
                }
            }
            chip->draw_flag = true;
        } break;

        default: 
            fprintf(stderr, "Encountered: 0x%02x%02x\n", *code, *(code+1));
            chip->is_running = false; 
            break;
    }
}
