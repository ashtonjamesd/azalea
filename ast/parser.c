#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "ast.h"
#include "symbol_table.h"

static Ast *init_ast() {
    Ast *ast = (Ast *)malloc(sizeof(Ast));
    if (!ast) return NULL;

    ast->body = (Expression *)malloc(sizeof(Expression));
    if (!ast->body) {
        free(ast);
        return NULL;
    }

    ast->expression_capacity = 1;
    ast->expression_count = 0;
    return ast;
}


ParserState *init_parser(LexerToken *tokens) {
    ParserState *state = (ParserState *)malloc(sizeof(ParserState));
    state->ast = init_ast();
    state->tokens = tokens;
    state->current = 0;
    state->error = NULL;

    return state;
}

static int is_end(ParserState *state) {
    if (state->tokens[state->current].type == TOKEN_EOF) {
        return 1;
    }

    return 0;
}

static inline void advance(ParserState *state) {
    state->current++;
}

static LexerToken get_current(ParserState *state) {
    return state->tokens[state->current];
}

static LexerToken peek(ParserState *state) {
    return state->tokens[state->current + 1];
}

static void raise_error(ParserState *state, char *expected, LexerToken token) {
    size_t error_size = snprintf(NULL, 0, "Expected '%s' but got '%s' on line %d", expected, token.lexeme, token.line) + 1;
    char *error_message = (char *)malloc(error_size);
    
    if (error_message != NULL) {
        snprintf(error_message, error_size, "Expected '%s' but got '%s' on line %d", expected, token.lexeme, token.line);
    } else {
        error_message = "Memory allocation error for error message";
    }

    state->error = error_message;
}

static inline int expect(ParserState *state, LexerTokenType type, char *str) {
    LexerToken token = get_current(state);

    if (token.type == type) {
        advance(state);
        return 1;
    }

    raise_error(state, str, token);
    return 0;
}

static inline Expression *create_expression(ExpressionType type) {
    Expression *expr = (Expression *)malloc(sizeof(Expression));
    expr->type = type;

    return expr;
}

static Expression *parse_primary_expression(ParserState *state) {
    LexerToken token = get_current(state);

    if (token.type == TOKEN_NUMERIC) {
        Expression *expr = create_expression(NUMERIC_LITERAL);

        expr->as.num_expr.value = malloc(sizeof(int));
        *expr->as.num_expr.value = (int)strtol(token.lexeme, NULL, 10);

        advance(state);
        return expr;
    }

    else if (token.type == TOKEN_IDENTIFIER) {
        Expression *expr = create_expression(IDENTIFIER);
        expr->as.ident_expr.identifier = strdup(token.lexeme);
        advance(state);

        return expr;
    }
    else if (token.type == TOKEN_STRING) {
        Expression *expr = create_expression(STRING_LITERAL);
        expr->as.str_expr.value = strdup(token.lexeme);
        advance(state);

        return expr;
    }
    else if (token.type == TOKEN_CHAR) {
        Expression *expr = create_expression(CHAR_LITERAL);
        expr->as.char_expr.value = strdup(token.lexeme)[0];
        advance(state);

        return expr;
    }
    else if (token.type == TOKEN_FLOAT) {
        Expression *expr = create_expression(FLOAT_LITERAL);
        expr->as.flt_expr.value = (float)strtol(token.lexeme, NULL, 10);
        advance(state);

        return expr;
    }
    else if (token.type == TOKEN_FALSE || token.type == TOKEN_TRUE) {
        Expression *expr = create_expression(BOOL_LITERAL);
        expr->as.bool_expr.value = strcmp(token.lexeme, "true") == 0 ? 1 : 0;
        advance(state);

        return expr;
    }
    else {
        raise_error(state, "expression", token);
        return NULL;
    }
}

void free_expression(Expression *expr) {
    if (!expr) return;
    switch (expr->type) {
        case STRING_LITERAL:
            free(expr->as.str_expr.value);
            break;
        case IDENTIFIER:
            free(expr->as.ident_expr.identifier);
            break;
        case VARIABLE_DECLARATION:
            free(expr->as.var_decl.identifier);
            free_expression(expr->as.var_decl.expr);
            break;
        case FUNCTION_CALL:
            free(expr->as.func_call.identifier);
            for (int i = 0; i < expr->as.func_call.arg_count; i++) {
                free_expression(expr->as.func_call.arguments[i]);
            }
            free(expr->as.func_call.arguments);
            break;
        case ASSIGNMENT_EXPR:
            free(expr->as.assign_expr.identifier);
            free_expression(expr->as.assign_expr.expr);
        default:
            break;
    }
    free(expr);
}

static Expression *parse_function_call(ParserState *state) {
    char *module = NULL;
    if (peek(state).type == TOKEN_DOT) {
        module = get_current(state).lexeme;
        advance(state);
    }
    
    if (module != NULL) advance(state);
    char *func_name = get_current(state).lexeme;
    advance(state);

    if (!expect(state, TOKEN_LEFT_PAREN, "(")) return NULL;

    Expression **arguments = NULL;
    int arg_count = 0;

    state->current--;
    if (get_current(state).type != TOKEN_RIGHT_PAREN) {
        do {
            advance(state);
            
            if (get_current(state).type == TOKEN_RIGHT_PAREN) {
                break;
            }

            Expression *arg_expr = parse_primary_expression(state);

            arguments = realloc(arguments, (arg_count + 1) * sizeof(Expression *));
            arguments[arg_count++] = arg_expr;

            // printf("%d", *arguments[arg_count]->as.num_expr.value);

        } while (get_current(state).type == TOKEN_COMMA);
    }

    // if (arg_count != 0) {
    //     state->current--;
    // }

    // for (int i = 0; i < arg_count; i++) {
    //     printf("STR ARGS: %s", *arguments[i]->as.str_expr.value);
    // }

    if (!expect(state, TOKEN_RIGHT_PAREN, ")")) {
        for (int i = 0; i < arg_count; i++) {
            free_expression(arguments[i]);
        }
        free(arguments);
        return NULL;
    }

    if (!expect(state, TOKEN_SEMI_COLON, ";")) {
        for (int i = 0; i < arg_count; i++) {
            free_expression(arguments[i]);
        }
        free(arguments);
        return NULL;
    }

    Expression *expr = create_expression(FUNCTION_CALL);
    expr->as.func_call.identifier = strdup(func_name);
    expr->as.func_call.arguments = arguments;
    expr->as.func_call.arg_count = arg_count;
    expr->as.func_call.module = strdup(module);

    return expr;
}

static VariableType map_token_type_to_var_type(LexerTokenType type) {
    switch (type) {
        case TOKEN_INT_TYPE:
            return VAR_TYPE_INT;
        case TOKEN_STRING_TYPE:
            return VAR_TYPE_STR;
        case TOKEN_BOOL_TYPE:
            return VAR_TYPE_BOOL;
        case TOKEN_FLOAT_TYPE:
            return VAR_TYPE_FLOAT;
        case TOKEN_CHAR_TYPE:
            return VAR_TYPE_CHAR;
        default:
            return VAR_TYPE_NULL;
    }
}

static Expression *parse_variable_declaration(ParserState *state) {
    if (!expect(state, TOKEN_LET, "let")) return NULL;

    // int is_mutable = 0;
    // if (get_current(state).type == TOKEN_MUT) {
    //     is_mutable = 1;
    //     advance(state);
    // }

    LexerToken identifier = get_current(state);
    if (!expect(state, TOKEN_IDENTIFIER, "identifier")) {
        return NULL;
    }

    VariableType type = VAR_TYPE_NULL;
    if (get_current(state).type == TOKEN_COLON) {
        advance(state);

        LexerToken typeIdentifier = get_current(state);
        type = map_token_type_to_var_type(typeIdentifier.type);
        advance(state);
    }

    if (!expect(state, TOKEN_SINGLE_EQUALS, "=")) {
        return NULL;
    }

    Expression *expr = create_expression(VARIABLE_DECLARATION);
    expr->as.var_decl.identifier = strdup(identifier.lexeme);
    expr->as.var_decl.type = type;

    if (get_current(state).type == TOKEN_IDENTIFIER && 
        (peek(state).type == TOKEN_DOT || peek(state).type == TOKEN_LEFT_PAREN)) {
        expr->as.var_decl.expr = parse_function_call(state);
    }
    else {
        expr->as.var_decl.expr = parse_primary_expression(state);

        if (!expect(state, TOKEN_SEMI_COLON, ";")) {
            return NULL;
        }
    }

    return expr;
}

static Expression *parse_assignment_expr(ParserState *state) {
    LexerToken identifier = get_current(state);
    advance(state);

    if (!expect(state, TOKEN_SINGLE_EQUALS, "=")) {
        return NULL;
    }

    Expression *assigned = parse_primary_expression(state);

    Expression *expr = create_expression(ASSIGNMENT_EXPR);
    expr->as.assign_expr.expr = assigned;
    expr->as.assign_expr.identifier = identifier.lexeme;

    if (!expect(state, TOKEN_SEMI_COLON, ";")) {
        return NULL;
    }

    return expr;
}

static Expression *parse_identifier(ParserState *state) {
    if (peek(state).type == TOKEN_SINGLE_EQUALS) {
        return parse_assignment_expr(state);
    } else {
        return parse_function_call(state);
    }
}

static Expression *parse_use_stmt(ParserState *state) {
    advance(state);

    LexerToken module = get_current(state);
    advance(state);

    Expression *expr = create_expression(USE_MODULE_STMT);
    expr->as.use_mod_expr.module = strdup(module.lexeme);

    if (!expect(state, TOKEN_SEMI_COLON, ";")) {
        return NULL;
    }

    return expr;
}

static Expression *parse_statement(ParserState *state) {
    LexerToken token = get_current(state);

    switch (token.type) {
        case TOKEN_LET:
            return parse_variable_declaration(state);
        case TOKEN_IDENTIFIER:
            return parse_identifier(state);
        case TOKEN_USE:
            return parse_use_stmt(state);
        default:
            return parse_primary_expression(state);
    }
}


void parse_tokens(ParserState *state) {
    while (!is_end(state)) {
        Expression *expr = parse_statement(state);

        if (!expr || state->error != NULL) {
            return;
        }

        if (state->ast->expression_count >= state->ast->expression_capacity) {
            state->ast->expression_capacity *= 2;
            state->ast->body = (Expression *)realloc(state->ast->body, sizeof(Expression) * state->ast->expression_capacity);
        }

        state->ast->body[state->ast->expression_count++] = *expr;

        if (is_end(state) || get_current(state).type == TOKEN_EOF) {
            if (state->debug_mode) {
                print_ast_body(state->ast);
            }

            break;
        }
    }
}

void parser_state_free(ParserState *state) {
    free(state->tokens);
    free(state);
}