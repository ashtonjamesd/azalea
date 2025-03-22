#include <stdio.h>
#include "parser.h"

void print_ast_recursive(Expression *expr, int depth) {
    if (expr == NULL) {
        for (int i = 0; i < depth; i++) {
            printf("  ");
        }
        printf("NULL Expression\n");
        return;
    }

    for (int i = 0; i < depth; i++) {
        printf("  ");
    }


    switch (expr->type) {
        case NUMERIC_LITERAL:
            printf("NUMERIC_LITERAL: %d\n", *expr->as.num_expr.value);
            break;

        case IDENTIFIER:
            printf("IDENTIFIER: %s\n", expr->as.ident_expr.identifier);
            break;

        case STRING_LITERAL:
            printf("STRING_LITERAL: %s\n", expr->as.str_expr.value);
            break;

        case FLOAT_LITERAL:
            printf("FLOAT_LITERAL: %f\n", expr->as.flt_expr.value);
            break;

        case BOOL_LITERAL:
            printf("BOOL_LITERAL: %d\n", expr->as.bool_expr.value);
            break;

        case CHAR_LITERAL:
            printf("CHAR_LITERAL: %c\n", expr->as.char_expr.value);
            break;

        case VARIABLE_DECLARATION:
            printf("VARIABLE_DECLARATION: %s | TYPE: %d\n", expr->as.var_decl.identifier, expr->as.var_decl.type);
            if (expr->as.var_decl.expr != NULL) {
                print_ast_recursive(expr->as.var_decl.expr, depth + 1);
            }
            break;

        case ASSIGNMENT_EXPR:
            printf("ASSIGNMENT_EXPR: %s\n", expr->as.assign_expr.identifier);
            if (expr->as.assign_expr.expr != NULL) {
                print_ast_recursive(expr->as.var_decl.expr, depth + 1);
            }
            break;

        case FUNCTION_CALL:
            printf("FUNCTION_CALL: [%s].%s\n", expr->as.func_call.module, expr->as.func_call.identifier);
            for (int i = 0; i < expr->as.func_call.arg_count; i++) {
                print_ast_recursive(expr->as.func_call.arguments[i], depth + 1);
            }
            break;

        case FUNCTION_DEFINITION:
            printf("FUNCTION DEFINITION: [%s]\n", expr->as.func_def.identifier);
            for (int i = 0; i < expr->as.func_def.statement_count; i++) {
                print_ast_recursive(expr->as.func_def.body[i], depth + 1);
            }
            break;

        case USE_MODULE_STMT:
            printf("USE_MODULE_STMT: %s", expr->as.use_mod_expr.module);
            break;

        default:
            printf("Unknown Expression Type\n");
            break;
    }
}

void print_ast_body(Ast *ast) {
    printf("\n");

    if (ast == NULL || ast->body == NULL) {
        printf("AST or AST body is NULL\n");
        return;
    }

    for (int i = 0; i < ast->expression_count; i++) {
        printf("\nExpression %d:\n", i + 1);
        print_ast_recursive(&ast->body[i], 1);
    }
}
