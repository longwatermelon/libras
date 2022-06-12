CC=gcc
CFLAGS=-std=gnu17 -ggdb -Wall -Werror -Wpedantic
LIBS=-lm -lSDL2

SRC=$(wildcard src/*.c)
OBJS=$(addprefix obj/, $(SRC:.c=.o))

all:
	mkdir -p obj/src lib
	$(MAKE) example

lib: $(OBJS)
	$(AR) $(ARFLAGS) lib/libras.a $^

example: main.c lib
	$(CC) $(CFLAGS) -Isrc main.c $(LIBS) -Llib -lras
	
obj/src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)

