SHELL := /bin/bash
CC = gcc
CFLAGS = -Iinclude -Ipivot -Ilexer -Iast -Itests -Iregistry -Wall -Wextra -g
SRC = pivot/pivot.c pivot/pivot_state.c lexer/state.c lexer/lexer.c lexer/token.c ast/parser.c ast/ast.c tests/lexer_tests.c tests/tests.c registry/function_registry.c
OBJ = $(SRC:.c=.o)
EXEC = main

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@