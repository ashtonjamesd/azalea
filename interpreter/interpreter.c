#include <stdio.h>
#include <stdlib.h>

#include "interpreter.h"
#include "function_registry.h"


static PivotInterpreter *init_interpreter(Ast *ast) {
    PivotInterpreter *interpreter = (PivotInterpreter *)malloc(sizeof(PivotInterpreter));
    interpreter->ast = ast;

    return interpreter;
}

void execute_function_call(PivotInterpreter *interpreter, Expression *expr) {
    FunctionPointer func = get_function(expr->as.func_call.identifier);
    if (func == NULL) {
        printf("undefined function '%s'", expr->as.func_call.identifier);
        return;
    }

    if (expr->as.func_call.arguments[0]->type == STRING_LITERAL) {
        func(expr->as.func_call.arguments[0]->as.str_expr.value);
    }
}

void interpret_ast(Ast *ast) {
    PivotInterpreter *interpreter = init_interpreter(ast);
    
    for (int i = 0; i < interpreter->ast->expression_count; i++) {
        execute_function_call(interpreter, &interpreter->ast->body[i]);
    }
}