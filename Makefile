SHELL := /bin/bash
CC = gcc
CFLAGS = -Iinclude -Ipivot -Ilexer -Iast -Wall -Wextra -g
SRC = pivot/pivot.c lexer/state.c pivot/pivot_state.c lexer/lexer.c lexer/token.c lexer/error.c ast/parser.c ast/ast.c
OBJ = $(SRC:.c=.o)
EXEC = main

all: $(EXEC)
	make clean

$(EXEC): $(OBJ)
	$(CC) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f pivot/*.o
	rm -f lexer/*.o
	rm -f ast/*.o
