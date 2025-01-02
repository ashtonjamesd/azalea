#include <stdio.h>

#include "parser.h"

void print_ast(Expression *expr, int depth) {
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }

    if (expr->type == NUMERIC_LITERAL) {
        printf("NUMERIC_LITERAL: %d\n", expr->as.num_expr.value);
    }
    else if (expr->type == IDENTIFIER) {
        printf("IDENTIFIER: %s\n", expr->as.ident_expr.identifier);
    }
    else if (expr->type == STRING_LITERAL) {
        printf("STRING_LITERAL: %s\n", expr->as.str_expr.value);
    }
    else if (expr->type == FLOAT_LITERAL) {
        printf("FLOAT_LITERAL: %f\n", *expr->as.flt_expr.value);
    }
    else if (expr->type == CHAR_LITERAL) {
        printf("CHAR_LITERAL: %c\n", expr->as.char_expr.value);
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
