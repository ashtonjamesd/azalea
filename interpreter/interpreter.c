#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "interpreter.h"
#include "function_registry.h"
#include "symbol_table.h"


static PivotInterpreter *init_interpreter(Ast *ast) {
    PivotInterpreter *interpreter = (PivotInterpreter *)malloc(sizeof(PivotInterpreter));
    interpreter->symbols = init_symbol_table();
    interpreter->ast = ast;
    interpreter->used_modules_count = 0;
    interpreter->has_error = 0;

    return interpreter;
}

static void set_error(PivotInterpreter *interpreter) {
    interpreter->has_error = 1;
}

static void raise_warning(const char *message, ...) {
    va_list args;
    va_start(args, message);

    printf("\n");
    
    printf("warning: ");
    vprintf(message, args);

    va_end(args);
}

static void raise_error(PivotInterpreter *interpreter, const char *message, ...) {
    va_list args;
    va_start(args, message);

    printf("\n");
    vprintf(message, args);
    printf("\n");

    va_end(args);

    set_error(interpreter);
}

static VariableType map_expr_type_to_var_type(ExpressionType type) {
    switch (type) {
        case NUMERIC_LITERAL:
            return VAR_TYPE_INT;

        case STRING_LITERAL:
            return VAR_TYPE_STR;

        case IDENTIFIER:
            return VAR_TYPE_NULL;

        default:
            printf("Unknown expression type when mapping to variable type");
            return VAR_TYPE_NULL;
    }
}

static int check_declaration_type(Expression *expr) {
    if (expr->as.var_decl.type == VAR_TYPE_NULL) {
        return 1;
    }

    if (expr->as.var_decl.expr->type == FUNCTION_CALL) {
        FunctionRegistryEntry *entry = get_function(
            expr->as.var_decl.expr->as.func_call.module,
            expr->as.var_decl.expr->as.func_call.identifier
        );

        if (entry->return_type == VAR_TYPE_STR && expr->as.var_decl.type != VAR_TYPE_STR) {
            return 0;
        }

        if (entry->return_type == VAR_TYPE_INT && expr->as.var_decl.type != VAR_TYPE_INT) {
            return 0;
        }

        return 1;
    }

    if (expr->as.var_decl.type == VAR_TYPE_STR && expr->as.var_decl.expr->type != STRING_LITERAL) {
        return 0;
    }

    if (expr->as.var_decl.type == VAR_TYPE_INT && expr->as.var_decl.expr->type != NUMERIC_LITERAL) {
        return 0;
    }

    if (expr->as.var_decl.type == VAR_TYPE_BOOL && expr->as.var_decl.expr->type != BOOL_LITERAL) {
        return 0;
    }

    if (expr->as.var_decl.type == VAR_TYPE_CHAR && expr->as.var_decl.expr->type != CHAR_LITERAL) {
        return 0;
    }

    if (expr->as.var_decl.type == VAR_TYPE_FLOAT && expr->as.var_decl.expr->type != FLOAT_LITERAL) {
        return 0;
    }

    return 1;
}

static void *execute_function_call(PivotInterpreter *interpreter, Expression *expr) {
    int has_module = 0;
    
    for (int i = 0; i < interpreter->used_modules_count; i++) {
        if (expr->as.func_call.module == NULL) break;

        if (strcmp(interpreter->used_modules[i], expr->as.func_call.module) == 0) {
            has_module = 1;
            break;
        }
    }
    
    if (!has_module && expr->as.func_call.module != NULL) {
        raise_error(interpreter, "undefined module '%s'", expr->as.func_call.module);
        return NULL;
    }
    
    FunctionRegistryEntry *entry = get_function(expr->as.func_call.module, expr->as.func_call.identifier);
    if (entry == NULL) {
        raise_error(interpreter, "undefined function '%s'", expr->as.func_call.identifier);
        return NULL;
    }

    if (entry->param_count != expr->as.func_call.arg_count) {
        raise_error(
            interpreter,
            "function '%s' takes %d parameters, found %d", 
            expr->as.func_call.identifier,
            entry->param_count,
            expr->as.func_call.arg_count
        );
        set_error(interpreter);
        
        return NULL;
    }

    for (int i = 0; i < expr->as.func_call.arg_count; i++) {
        VariableType type = map_expr_type_to_var_type(expr->as.func_call.arguments[i]->type);

        if (expr->as.func_call.arguments[i]->type == IDENTIFIER) {
            VariableSymbol *var = get_variable(interpreter->symbols, expr->as.func_call.arguments[i]->as.ident_expr.identifier);
            if (var == NULL) {
                raise_error(interpreter, "undefined variable '%s' passed as parameter", expr->as.func_call.arguments[i]->as.ident_expr.identifier);
                return NULL;
            }
            type = var->type;
        }
        
        if (type != entry->param_types[i]) {
            raise_error(interpreter, "incorrect type for argument %d of function '%s'", i + 1, expr->as.func_call.identifier);
            return NULL;
        }
    }

    void *args[expr->as.func_call.arg_count];

    for (int i = 0; i < expr->as.func_call.arg_count; i++) {
        if (expr->as.func_call.arguments[i]->type == STRING_LITERAL) {
            args[i] = (void *)expr->as.func_call.arguments[i]->as.str_expr.value;
        }
        else if (expr->as.func_call.arguments[i]->type == NUMERIC_LITERAL) {
            args[i] = (void *)expr->as.func_call.arguments[i]->as.num_expr.value;
        }
        else if (expr->as.func_call.arguments[i]->type == IDENTIFIER) {
            char *name = expr->as.func_call.arguments[i]->as.ident_expr.identifier;

            VariableSymbol *symbol = get_variable(interpreter->symbols, name);
            if (symbol == NULL) {
                raise_error(interpreter, "undefined variable %s", name);
            }
            
            if (symbol->type == VAR_TYPE_STR) {
                args[i] = symbol->as.str_val;
            }
            else if (symbol->type == VAR_TYPE_INT) {
                args[i] = symbol->as.int_val;
            }
            else {
                printf("Unable to assign identifier type in function call");
            }
        }
    }

    void *result = entry->func(args);
    if (!result && entry->return_type != VAR_TYPE_NULL) {
        printf("function '%s' returned null", entry->name);
        set_error(interpreter);
    }

    return result;
}

void execute_variable_declaration(PivotInterpreter *interpreter, Expression *expr) {
    VariableSymbol *var = get_variable(interpreter->symbols, expr->as.var_decl.identifier);
    if (var != NULL) {
        raise_error(interpreter, "redeclaration of variable '%s'", expr->as.var_decl.identifier);
        return;
    }

    int is_valid_type = check_declaration_type(expr);
    if (!is_valid_type) {
        raise_error(interpreter, "mismatching type on variable declaration");
        return;
    }

    if (expr->as.var_decl.expr->type == FUNCTION_CALL) {
        FunctionRegistryEntry *entry = get_function(
            expr->as.var_decl.expr->as.func_call.module,
            expr->as.var_decl.expr->as.func_call.identifier
        );

        if (entry == NULL) {
            set_error(interpreter);
            
            if (expr->as.var_decl.expr->as.func_call.module == NULL) {
                raise_warning(
                    "undefined function '%s'", 
                    expr->as.var_decl.expr->as.func_call.identifier
                );
            } else {
                raise_warning(
                    "undefined function '%s' in module '%s'", 
                    expr->as.var_decl.expr->as.func_call.identifier, 
                    expr->as.var_decl.expr->as.func_call.module
                );
            }

            return;
        }

        if (entry->return_type == VAR_TYPE_NULL) {
            raise_error(interpreter, "cannot assign void to variable");
            return;
        }
    }

    if (expr->as.var_decl.expr->type == STRING_LITERAL) {
        set_variable(interpreter->symbols, expr->as.var_decl.identifier, VAR_TYPE_STR, expr->as.var_decl.expr->as.str_expr.value);
    }
    else if (expr->as.var_decl.expr->type == FUNCTION_CALL) {
        void *result = execute_function_call(interpreter, expr->as.var_decl.expr);

        FunctionRegistryEntry *func = get_function(expr->as.var_decl.expr->as.func_call.module, expr->as.var_decl.expr->as.func_call.identifier);
        if (func == NULL) {
            printf("Null function when setting variable");
            set_error(interpreter);
            return;
        }

        if (func->return_type == VAR_TYPE_STR) {
            set_variable(interpreter->symbols, expr->as.var_decl.identifier, VAR_TYPE_STR, (char *)result);
        }
        else if (func->return_type == VAR_TYPE_INT) {
            set_variable(interpreter->symbols, expr->as.var_decl.identifier, VAR_TYPE_INT, (int *)result);
        }
        else {
            printf("Unable to set variable type from function. Bad type.");
        }
    }
    else if (expr->as.var_decl.expr->type == NUMERIC_LITERAL) {
        set_variable(interpreter->symbols, expr->as.var_decl.identifier, VAR_TYPE_INT, expr->as.var_decl.expr->as.num_expr.value);
    }
    else {
        printf("Unable to set variable. Bad type.");
    }
}

void execute_assignment_expression(PivotInterpreter *interpreter, Expression *expr) {
    VariableSymbol *var = get_variable(interpreter->symbols, expr->as.assign_expr.identifier);
    if (var == NULL) {
        raise_error(interpreter, "undefined variable %s", expr->as.assign_expr.identifier);
        return;
    }

    if (!var->is_mutable) {
        raise_error(interpreter, "immutable variable %s cannot be reassigned", expr->as.assign_expr.identifier);
        return;
    }

    Expression *assign = expr->as.assign_expr.expr;

    if (assign->type == STRING_LITERAL) {
        set_variable(interpreter->symbols, expr->as.assign_expr.identifier, VAR_TYPE_STR, expr->as.var_decl.expr->as.str_expr.value);
    }
    else if (assign->type == IDENTIFIER) {
        VariableSymbol *var = get_variable(interpreter->symbols, assign->as.ident_expr.identifier);
        if (var == NULL) {
            raise_error(interpreter, "undefined variable %s", assign->as.ident_expr.identifier);
            return;
        }

        set_variable(interpreter->symbols, expr->as.assign_expr.identifier, VAR_TYPE_STR, var->as.str_val);
    }
}

void execute_use_module_stmt(PivotInterpreter *interpreter, Expression *expr) {    
    for (int i = 0; i < interpreter->used_modules_count; i++) {
        if (strcmp(interpreter->used_modules[i], expr->as.use_mod_expr.module) == 0) {
            raise_warning("module '%s' already used", interpreter->used_modules[i]);
            return;
        }
    }

    FunctionRegistryModule *module = get_module(expr->as.use_mod_expr.module);
    if (module == NULL) {
        raise_error(interpreter, "module '%s' undefined", expr->as.use_mod_expr.module);
        return;
    }

    int module_is_used = 0;
    for (int i = 0; i < interpreter->ast->expression_count; i++) {
        if (interpreter->ast->body[i].type == FUNCTION_CALL) {
            if (interpreter->ast->body[i].as.func_call.module == NULL) continue;

            if (strcmp(interpreter->ast->body[i].as.func_call.module, expr->as.use_mod_expr.module) == 0) {
                module_is_used = 1;
            }
        }
        else if (interpreter->ast->body[i].type == VARIABLE_DECLARATION) {
            if (interpreter->ast->body[i].as.var_decl.expr->type == FUNCTION_CALL) {
                if (interpreter->ast->body[i].as.var_decl.expr->as.func_call.module == NULL) continue;

                if (strcmp(interpreter->ast->body[i].as.var_decl.expr->as.func_call.module, expr->as.use_mod_expr.module) == 0) {
                    module_is_used = 1;
                }
            }
        }
    }

    if (!module_is_used) {
        raise_warning("module '%s' not used", expr->as.use_mod_expr.module);
    }

    interpreter->used_modules[interpreter->used_modules_count++] = expr->as.use_mod_expr.module;
}

void execute_function_definition(PivotInterpreter *interpreter, Expression *expr) {

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

        case FUNCTION_DEFINITION:
            execute_function_definition(interpreter, expr);
            break;

        default:
            printf("Unknown interpreter expression: %d", expr->type);
    }
}

void interpret_ast(Ast *ast) {
    PivotInterpreter *interpreter = init_interpreter(ast);

    for (int i = 0; i < interpreter->ast->expression_count; i++) {
        execute_statement(interpreter, &interpreter->ast->body[i]);

        if (interpreter->has_error) {
            break;
        }
    }

    printf("\n\n");
}