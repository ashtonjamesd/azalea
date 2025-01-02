#include "token.h"

#ifndef LEXER_STATE_H
#define LEXER_STATE_H

typedef struct LexerState {
    char *source;
    int current;
    char *error;
    LexerToken *tokens;
    int token_count;
    int token_capacity;
    int line;
} LexerState;

extern void lexer_state_free(LexerState *state);

#endif