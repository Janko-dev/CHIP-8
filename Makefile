CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -g -lm
SDL=-IC:\Users\janba\Dev\i686-w64-mingw32\include -LC:\Users\janba\Dev\i686-w64-mingw32\lib -lmingw32 -lSDL2main -lSDL2
IN = chip8.c main.c
OUT = out

default:
	$(CC) $(IN) -o $(OUT) $(CFLAGS) $(SDL)