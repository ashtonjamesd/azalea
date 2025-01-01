SHELL := /bin/bash
CC = gcc
CFLAGS = -Iinclude -Ipivot -Iparser -Wall -Wextra -g
SRC = pivot/pivot.c parser/state.c pivot/pivot_state.c parser/lexer.c
OBJ = $(SRC:.c=.o)
EXEC = main

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f parser/*.o main.exe
	rm -f pivot/*.o
