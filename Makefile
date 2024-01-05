CC = gcc
CFLAGS = -Wall -Wextra -std=c18 -O0 -ggdb # -fsanitize=address

SRC = $(wildcard *.c)
OBJ = $(patsubst %.c, obj/%.o, $(SRC))

all: build pacman

build:
	mkdir -p obj

pacman: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -lSDL2 -lm

obj/%.o: %.c
	$(CC) $(CFLAGS) -o $@ $^ -c

clean:
	rm -rf obj pacman

run: pacman
	./pacman
