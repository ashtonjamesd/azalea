#include "ast.h"

#ifndef INTERPRETER_H
#define INTERPRETER_H

typedef struct PivotInterpreter {
    Ast *ast;
} PivotInterpreter;

extern void interpret_ast(Ast *ast);

#endif