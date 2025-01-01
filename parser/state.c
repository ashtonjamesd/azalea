#include <stdlib.h>

#include "state.h"

void lexer_state_free(LexerState *state) {
    free(state->source);
    free(state->error);
    free(state);
}