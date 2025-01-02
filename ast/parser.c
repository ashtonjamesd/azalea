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
    else {

        printf("unknown primary expression");
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

    if (!expect(state, TOKEN_SEMI_COLON)) return NULL;
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

        if (state->error == NULL || expr == NULL) {
            printf(state->error);
            return state;
        }

        state->ast->body[state->ast->expression_count++] = *expr;

        if (is_end(state)) {
            goto end;
        }
    }
    
    end:
    printf("\n");
    print_ast(state->ast->body, 0);
    
    return state;
}

void parser_state_free(ParserState *state) {
    free(state->tokens);
    free(state);
}