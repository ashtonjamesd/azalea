#ifndef LEXER_STATE_H
#define LEXER_STATE_H

typedef struct LexerState {
    char *source;
    int current;
    int statuscode;
    char *error;
} LexerState;

extern void lexer_state_free(LexerState *state);

#endif