#include <stdio.h>
#include <stdlib.h>

#include "interpreter.h"
#include "function_registry.h"
#include "symbol_table.h"


static PivotInterpreter *init_interpreter(Ast *ast) {
    PivotInterpreter *interpreter = (PivotInterpreter *)malloc(sizeof(PivotInterpreter));
    interpreter->symbols = init_symbol_table();
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

void execute_variable_declaration(PivotInterpreter *interpreter, Expression *expr) {
    if (expr->as.var_decl.expr->type == STRING_LITERAL) {
        set_variable(interpreter->symbols, expr->as.var_decl.identifier, VAR_TYPE_STR, expr->as.var_decl.expr->as.str_expr.value);
    }
}

void execute_statement(PivotInterpreter *interpreter, Expression *expr) {
    switch (expr->type) {
        case VARIABLE_DECLARATION:
            execute_variable_declaration(interpreter, expr);
            break;

        case FUNCTION_CALL:
            execute_function_call(interpreter, expr);
            break;

        default:
            printf("Unknown interpreter expression");
    }
}

void interpret_ast(Ast *ast) {
    PivotInterpreter *interpreter = init_interpreter(ast);

    for (int i = 0; i < interpreter->ast->expression_count; i++) {
        execute_statement(interpreter, &interpreter->ast->body[i]);
    }
}