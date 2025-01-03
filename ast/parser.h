#include "token.h"
#include "ast.h"

#ifndef PARSER_H
#define PARSER_H

typedef struct ParserState {
    LexerToken *tokens;
    int current;
    char *error;
    Ast *ast;

    int debug_mode;
} ParserState;

extern void parse_tokens(ParserState *state);
extern void parser_state_free(ParserState *state);
extern ParserState *init_parser(LexerToken *tokens);

#endif