CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -g
IN = main.c
OUT = out

default:
	$(CC) $(IN) -o $(OUT) $(CFLAGS)