#include "ast.h"
#include "symbol_table.h"

#ifndef INTERPRETER_H
#define INTERPRETER_H

typedef struct PivotInterpreter {
    Ast *ast;
    SymbolTable *symbols;
} PivotInterpreter;

extern void interpret_ast(Ast *ast);

#endif