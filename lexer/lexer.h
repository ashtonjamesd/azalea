#include "token.h"

#ifndef LEXER_H
#define LEXER_H

typedef struct LexerState {
    char *source;
    int current;
    char *error;
    LexerToken *tokens;
    int token_count;
    int token_capacity;
    int line;

    // set to 1 if the tokens should be written to stdout
    int debug;
    
} LexerState;

extern void tokenize_file(LexerState *state, char *path);
extern LexerState *init_lexer_state();
extern void lexer_state_free(LexerState *state);

#endif