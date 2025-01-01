#include <stdio.h>

#include "error.h"
#include "state.h"

void set_error(struct LexerState *state, LexerError error) {
    state->error = error;
}

char *out_error(LexerError error) {
    switch (error) {
        case LEXER_OK: return "No Error";
        case LEXER_FILE_ERROR: return "Error reading file";
        case LEXER_MEMORY_ERROR: return "Error allocating memory";
        case LEXER_UNEXPECTED_CHARACTER_ERROR: return "Unexpected character";
    }

    return "Unknown Error";
}
