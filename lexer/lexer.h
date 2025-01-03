#include "state.h"

#ifndef LEXER_H
#define LEXER_H

extern void tokenize_file(LexerState *state, char *path);
extern LexerState *init_lexer_state();

#endif