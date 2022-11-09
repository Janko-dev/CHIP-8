#ifndef _CHIP8_H
#define _CHIP8_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#include "SDL2/SDL.h"
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_audio.h>

#define HIGH(byte) ((byte) & 0xf0)
#define LOW(byte)  ((byte) & 0xf)

#define TITLE "CHIP-8 Emulator"
#define WIDTH 640
#define HEIGHT 320

#define PIXEL_WIDTH 64
#define PIXEL_HEIGHT 32

#define AMPLITUDE 28000
#define SAMPLE_RATE 44100

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
} SDL_State;

typedef struct {
    uint8_t reg[16];
    uint8_t mem[0xffff];
    uint16_t stack[0xff];
    uint8_t keys[16];
    uint8_t pixels[PIXEL_WIDTH * PIXEL_HEIGHT];
    uint16_t sp;
    uint16_t pc;
    uint16_t I;
    uint8_t delay_timer;
    uint8_t sound_timer;
    bool is_running;
    bool draw_flag;
    SDL_State state;
} CHIP8;

CHIP8* create_CHIP8();
void destroy_CHIP8(CHIP8* chip);
void load_ROM(CHIP8* chip, const char* file_path);
void handle_events(CHIP8* chip);
void render(CHIP8* chip);
void interpret(CHIP8* chip);

#endif //_CHIP8_H
