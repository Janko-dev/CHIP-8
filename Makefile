CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -g -lm
IN = chip8.c main.c
OUT = chip8

ifeq ($(OS), Windows_NT)
	SDL=-IC:\Users\janba\Dev\i686-w64-mingw32\include -LC:\Users\janba\Dev\i686-w64-mingw32\lib -lmingw32 -lSDL2main -lSDL2
else 
	SDL=`pkg-config --cflags --libs sdl2`
endif

default:
	$(CC) $(IN) -o $(OUT) $(CFLAGS) $(SDL)