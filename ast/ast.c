#include <stdio.h>

#include "parser.h"

void print_ast(Expression *expr, int depth) {
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }

    if (expr->type == NUMERIC_LITERAL) {
        printf("NUMERIC_LITERAL: %d\n", expr->as.num_expr.value);
    } 
    else if (expr->type == VARIABLE_DECLARATION) {
        printf("VARIABLE_DECLARATION: %s", expr->as.var_decl.identifier);
        if (expr->as.var_decl.expr != NULL) {
            printf(" = ");
            print_ast(expr->as.var_decl.expr, depth + 1);
        } else {
            printf("\n");
        }
    } 
    else {
        printf("Unknown Expression Type\n");
    }
}
