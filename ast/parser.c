#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "ast.h"

static Ast *init_ast() {
    Ast *ast = (Ast *)malloc(sizeof(Ast));
    ast->body = (Expression *)malloc(sizeof(Expression) * 2);
    ast->expression_capacity = 2;
    ast->expression_count = 0;

    return ast;
}

static ParserState *init_parser(LexerToken *tokens) {
    ParserState *state = (ParserState *)malloc(sizeof(ParserState));
    state->ast = init_ast();
    state->tokens = tokens;
    state->current = 0;

    return state;
}

static int is_end(ParserState *state) {
    if (state->tokens[state->current].type == TOKEN_EOF) {
        return 0;
    }

    return 1;
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

Expression *parse_primary_expression(ParserState *state) {
    LexerToken token = get_current(state);

    if (token.type == TOKEN_NUMERIC) {
        Expression *expr = create_expression(NUMERIC_LITERAL);
        expr->as.num_expr.value = (int)strtol(token.lexeme, NULL, 10);
        return expr;
    }

    advance(state);
}


Expression *parse_variable_declaration(ParserState *state) {
    if (!expect(state, TOKEN_LET)) return NULL;

    LexerToken identifier = get_current(state);
    advance(state);

    advance(state);

    Expression *expr = create_expression(VARIABLE_DECLARATION);
    expr->as.var_decl.identifier = strdup(identifier.lexeme);
    expr->as.var_decl.expr = parse_primary_expression(state);

    if (!expect(state, TOKEN_SEMI_COLON)) return NULL;
    return expr;
}

void parse_statement(ParserState *state) {
    parse_variable_declaration(state);
}

ParserState *parse_tokens(LexerToken *tokens) {
    ParserState *state = init_parser(tokens);

    while (!is_end(state)) {
        parse_statement(state);

        if (is_end(state)) {
            return state;
        }

        if (state->error != NULL) {
            printf(state->error);
            return state;
        }
    }

    return state;
}

void parser_state_free(ParserState *state) {
    free(state->tokens);
    free(state);
}
