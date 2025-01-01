#ifndef LEXER_STATE_H
#define LEXER_STATE_H

typedef struct LexerState {
    char *source;
    int current;
} LexerState;

#endif