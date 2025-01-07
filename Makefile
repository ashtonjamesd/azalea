SHELL := /bin/bash
CC = gcc
CFLAGS = -Iinclude -Ipivot -Ilexer -Iast -Itests -Istdlib -Iinterpreter -Wall -Wextra -g
SRC = pivot/pivot.c pivot/pivot_state.c lexer/lexer.c lexer/token.c ast/parser.c ast/ast.c tests/lexer_tests.c tests/tests.c interpreter/function_registry.c stdlib/io.c interpreter/interpreter.c interpreter/symbol_table.c stdlib/math.c
OBJ = $(SRC:.c=.o)
EXEC = main

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $^ -o $@
	pwsh ./clean.ps1

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@