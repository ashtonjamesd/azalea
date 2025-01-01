#include "token.h"
#include "ast.h"

#ifndef PARSER_H
#define PARSER_H

typedef struct ParserState {
    LexerToken *tokens;
    int current;
    char *error;
    Ast *ast;
} ParserState;

extern ParserState *parse_tokens(LexerToken* tokens);
extern void parser_state_free(ParserState *state);

#endif