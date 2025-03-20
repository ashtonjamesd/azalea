SHELL := /bin/bash
CC = gcc
CFLAGS = -Iinclude -Iazalea -Ilexer -Iast -Itests -Istdlib -Iinterpreter -Wall -Wextra -g
SRC = azalea/azalea.c azalea/azalea_state.c lexer/lexer.c lexer/token.c ast/parser.c ast/ast.c tests/lexer_tests.c tests/tests.c interpreter/function_registry.c stdlib/io.c interpreter/interpreter.c interpreter/symbol_table.c stdlib/math.c stdlib/str.c
OBJ = $(SRC:.c=.o)
EXEC = main

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $^ -o $@
	pwsh ./clean.ps1

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@