#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

#define HIGH(byte) ((byte) & 0xf0)
#define LOW(byte)  ((byte) & 0xf)

typedef struct {
    uint8_t reg[16];
    uint8_t mem[0xffff];
    uint8_t* chunk;
    uint16_t pc;
} CHIP8;

CHIP8* create_CHIP8(){
    CHIP8* chip = (CHIP8*)calloc(1, sizeof(CHIP8));
    if (chip == NULL) return NULL;
    return chip;
}

void free_CHIP8(CHIP8* chip){
    free(chip->chunk);
    free(chip);
}

void load_ROM(CHIP8* chip, const char* file_path){
    FILE* f = fopen(file_path, "rb");
    fseek(f, 0L, SEEK_END);
    size_t size = ftell(f);
    fseek(f, 0L, SEEK_SET);
    chip->chunk = (uint8_t*)calloc(1, size);
    fread(chip->chunk, sizeof(uint8_t), size, f);
    fclose(f);
}

void interpret(CHIP8* chip){

    while (true) {
        uint8_t* code = chip->chunk + chip->pc;
        switch (HIGH(*code)){
            case 0x60: // 6XNN	Store number NN in register VX
                chip->reg[LOW(*code)] = *(code+1);
                break;
            case 0x80: 
                uint8_t arg = *(code+1);
                switch (LOW(arg)){
                    case 0x00: // 8XY0	Store the value of register VY in register VX
                        chip->reg[LOW(*code)] = chip->reg[HIGH(*(code+1)) >> 4];
                        break;
                    case 0x04:{ // 8XY4  Add the value of register VY to register VX. Set VF to 01 if a carry occurs. Set VF to 00 if a carry does not occur
                        uint8_t vx = chip->reg[LOW(*code)];
                        uint8_t vy = chip->reg[HIGH(arg) >> 4];
                        if (vx > UINT8_MAX - vy) chip->reg[0xf] = 1;
                        else chip->reg[0xf] = 0;
                        chip->reg[LOW(*code)] += vy;
                    } break;
                    case 0x05:{ // 8XY5	Subtract the value of register VY from register VX. Set VF to 00 if a borrow occurs. Set VF to 01 if a borrow does not occur
                        uint8_t vx = chip->reg[LOW(*code)];
                        uint8_t vy = chip->reg[HIGH(arg) >> 4];
                        if (vx < vy) chip->reg[0xf] = 0;
                        else chip->reg[0xf] = 1;
                        chip->reg[LOW(*code)] -= vy;
                    } break;
                    case 0x07:{ // 8XY7	Set register VX to the value of VY minus VX. Set VF to 00 if a borrow occurs. Set VF to 01 if a borrow does not occur
                        uint8_t vx = chip->reg[LOW(*code)];
                        uint8_t vy = chip->reg[HIGH(arg) >> 4];
                        if (vy < vx) chip->reg[0xf] = 0;
                        else chip->reg[0xf] = 1;
                        chip->reg[LOW(*code)] = vy - vx;
                    } break;
                    case 0x02: // 8XY2	Set VX to VX AND VY
                        chip->reg[LOW(*code)] &= chip->reg[HIGH(arg) >> 4]; break;
                    case 0x01: // 8XY1	Set VX to VX OR VY
                        chip->reg[LOW(*code)] |= chip->reg[HIGH(arg) >> 4]; break;
                    case 0x03: // 8XY3	Set VX to VX XOR VY
                        chip->reg[LOW(*code)] ^= chip->reg[HIGH(arg) >> 4]; break;
                    default: break;
                }
                    
                break;
            case 0x70: // 7XNN	Add the value NN to register VX
                chip->reg[LOW(*code)] += *(code+1);
            
            default: break;
        }
        chip->pc+=2;
    }
}

int main(void){

    CHIP8* chip = create_CHIP8();
    load_ROM(chip, "resources/test_opcode.ch8");



    free_CHIP8(chip);
    return 0;
}