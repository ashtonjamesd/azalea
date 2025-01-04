#include "ast.h"
#include "symbol_table.h"
#include "function_registry.h"

#ifndef INTERPRETER_H
#define INTERPRETER_H

typedef struct PivotInterpreter {
    Ast *ast;
    SymbolTable *symbols;
    char *used_modules[MAX_MODULES];
    int used_modules_count;
} PivotInterpreter;

extern void interpret_ast(Ast *ast);

#endif