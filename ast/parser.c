#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "ast.h"

static Ast *init_ast() {
    Ast *ast = (Ast *)malloc(sizeof(Ast));
    if (!ast) return NULL;
    ast->body = (Expression *)malloc(sizeof(Expression) * 2);
    if (!ast->body) {
        free(ast);
        return NULL;
    }
    ast->expression_capacity = 2;
    ast->expression_count = 0;
    return ast;
}


static ParserState *init_parser(LexerToken *tokens) {
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

static inline int expect(ParserState *state, LexerTokenType type) {
    if (get_current(state).type == type) {
        advance(state);
        return 1;
    }

    state->error = "Unexpected token";
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
        expr->as.num_expr.value = (int)strtol(token.lexeme, NULL, 10);
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
        state->error = "Unknown primary expression";
        return NULL;
    }
}

static Expression *parse_variable_declaration(ParserState *state) {
    if (!expect(state, TOKEN_LET)) return NULL;

    LexerToken identifier = get_current(state);
    advance(state);

    advance(state);

    Expression *expr = create_expression(VARIABLE_DECLARATION);
    expr->as.var_decl.identifier = strdup(identifier.lexeme);
    expr->as.var_decl.expr = parse_primary_expression(state);

    advance(state);

    return expr;
}

static Expression *parse_statement(ParserState *state) {
    LexerToken token = get_current(state);

    if (token.type == TOKEN_LET) {
        return parse_variable_declaration(state);
    } else {
        return parse_primary_expression(state);
    }
}

ParserState *parse_tokens(LexerToken *tokens) {
    ParserState *state = init_parser(tokens);

    while (!is_end(state)) {
        Expression *expr = parse_statement(state);

        if (state->error != NULL) {
            return state;
        }

        if (state->ast->expression_count >= state->ast->expression_capacity) {
            state->ast->expression_capacity *= 2;
            state->ast->body = (Expression *)realloc(state->ast->body, sizeof(Expression) * state->ast->expression_capacity);
        }

        state->ast->body[state->ast->expression_count++] = *expr;

        if (is_end(state) || get_current(state).type == TOKEN_EOF) {
            goto end;
        }
    }
    
    end:
    printf("\n");
    print_ast_body(state->ast);
    
    return state;
}

void parser_state_free(ParserState *state) {
    free(state->tokens);
    free(state);
}