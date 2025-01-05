#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interpreter.h"
#include "function_registry.h"
#include "symbol_table.h"


static PivotInterpreter *init_interpreter(Ast *ast) {
    PivotInterpreter *interpreter = (PivotInterpreter *)malloc(sizeof(PivotInterpreter));
    interpreter->symbols = init_symbol_table();
    interpreter->ast = ast;
    interpreter->used_modules_count = 0;

    return interpreter;
}

VariableType map_expr_type_to_var_type(ExpressionType type) {
    switch (type) {
        case NUMERIC_LITERAL:
            return VAR_TYPE_INT;

        case STRING_LITERAL:
        case IDENTIFIER:
            return VAR_TYPE_STR;

        default:
            printf("Unknown expression type when mapping to variable type");
            return VAR_TYPE_NULL;
    }
}

void execute_function_call(PivotInterpreter *interpreter, Expression *expr) {
    int has_module = 0;
    
    for (int i = 0; i < interpreter->used_modules_count; i++) {
        if (expr->as.func_call.module == NULL) break;

        if (strcmp(interpreter->used_modules[i], expr->as.func_call.module) == 0) {
            has_module = 1;
            break;
        }
    }
    
    if (!has_module && expr->as.func_call.module != NULL) {
        printf("undefined module '%s'", expr->as.func_call.module);
        return;
    }

    FunctionRegistryEntry *entry = get_function(expr->as.func_call.module, expr->as.func_call.identifier);
    if (entry == NULL) {
        printf("undefined function '%s'", expr->as.func_call.identifier);
        return;
    }

    if (entry->param_count != expr->as.func_call.arg_count) {
        printf(
            "function '%s' takes %d parameters, got %d", 
            expr->as.func_call.identifier,
            entry->param_count,
            expr->as.func_call.arg_count
        );
        return;
    }

    for (int i = 0; i < expr->as.func_call.arg_count; i++) {
        VariableType type = map_expr_type_to_var_type(expr->as.func_call.arguments[i]->type);
        
        if (type != entry->param_types[i]) {
            printf("incorrect type for argument %d of function '%s'", i + 1, expr->as.func_call.identifier);
            return;
        }
    }

    if (expr->as.func_call.arg_count == 0) {
        entry->func(NULL);
        return;
    }

    if (expr->as.func_call.arguments[0]->type == STRING_LITERAL) {
        entry->func(expr->as.func_call.arguments[0]->as.str_expr.value);
    }
    else if (expr->as.func_call.arguments[0]->type == IDENTIFIER) {
        char *name = expr->as.func_call.arguments[0]->as.ident_expr.identifier;

        VariableSymbol *symbol = get_variable(interpreter->symbols, name);
        if (symbol == NULL) {
            printf("undefined variable %s", name);
        }
        
        entry->func(symbol->as.str_val);
    }
}

void execute_variable_declaration(PivotInterpreter *interpreter, Expression *expr) {
    VariableSymbol *var = get_variable(interpreter->symbols, expr->as.var_decl.identifier);
    if (var != NULL) {
        printf("redeclaration of variable '%s'", expr->as.var_decl.identifier);
        return;
    }

    if (expr->as.var_decl.expr->type == STRING_LITERAL) {
        set_variable(interpreter->symbols, expr->as.var_decl.identifier, VAR_TYPE_STR, expr->as.var_decl.expr->as.str_expr.value);
    }
    else if (expr->as.var_decl.expr->type == FUNCTION_CALL) {
        execute_function_call(interpreter, expr->as.var_decl.expr);
    }
}

void execute_assignment_expression(PivotInterpreter *interpreter, Expression *expr) {
    VariableSymbol *var = get_variable(interpreter->symbols, expr->as.assign_expr.identifier);
    if (var == NULL) {
        printf("undefined variable %s", expr->as.assign_expr.identifier);
        return;
    }

    Expression *assign = expr->as.assign_expr.expr;

    if (assign->type == STRING_LITERAL) {
        set_variable(interpreter->symbols, expr->as.assign_expr.identifier, VAR_TYPE_STR, expr->as.var_decl.expr->as.str_expr.value);
    }
    else if (assign->type == IDENTIFIER) {
        VariableSymbol *var = get_variable(interpreter->symbols, assign->as.ident_expr.identifier);
        if (var == NULL) {
            printf("undefined variable %s", assign->as.ident_expr.identifier);
            return;
        }

        set_variable(interpreter->symbols, expr->as.assign_expr.identifier, VAR_TYPE_STR, var->as.str_val);
    }
}

void execute_use_module_stmt(PivotInterpreter *interpreter, Expression *expr) {
    for (int i = 0; i < interpreter->used_modules_count; i++) {
        if (strcmp(interpreter->used_modules[i], expr->as.use_mod_expr.module) == 0) {
            printf("module already used");
            return;
        }
    }

    interpreter->used_modules[interpreter->used_modules_count++] = expr->as.use_mod_expr.module;
}

void execute_statement(PivotInterpreter *interpreter, Expression *expr) {
    switch (expr->type) {
        case VARIABLE_DECLARATION:
            execute_variable_declaration(interpreter, expr);
            break;

        case FUNCTION_CALL:
            execute_function_call(interpreter, expr);
            break;

        case ASSIGNMENT_EXPR:
            execute_assignment_expression(interpreter, expr);
            break;

        case USE_MODULE_STMT:
            execute_use_module_stmt(interpreter, expr);
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