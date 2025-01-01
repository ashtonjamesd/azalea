#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "state.h"

static LexerState *init_lexer_state() {
    LexerState *state = (LexerState *)malloc(sizeof(LexerState));
    state->current = 0;
    state->statuscode = 0;
    state->error = NULL;

    return state;
}

static void read_source(LexerState *state, char *path) {
    FILE *fptr = fopen(path, "r");
    if (!fptr) {
        state->error = "Error opening source file.";
        return;
    }

    fseek(fptr, 0, SEEK_END);
    long sz = ftell(fptr);
    rewind(fptr);

    char *buff = (char *)malloc(sz);
    if (!buff) {
        state->error = "Error allocating memory for source buffer.";
        fclose(fptr);
        return;
    }

    fread(buff, 1, sz, fptr);
    buff[sz] = '\0';

    state->source = strdup(buff);
    
    fclose(fptr);
    free(buff);
}

LexerState *tokenize_file(char *path) {
    LexerState *state = init_lexer_state();
    read_source(state, path);

    if (state->error != NULL) {
        printf("Lexer Error: %s", state->error);
        return state;
    }


    
    printf("%s", state->source);

    return state;
}