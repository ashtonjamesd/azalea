#include <stdio.h>
#include <stdlib.h>

#include "pivot_state.h"
#include "lexer.h"
#include "error.h"

PivotState *init_pivot(int argc, char **argv) {
    PivotState *status = (PivotState *)malloc(sizeof(PivotState));
    if (!status) {
        perror("Error allocating memory for interpreter state");
        return NULL;
    }

    status->status_code = PIVOT_STATUS_OK;

    status->pivot_args = argv;
    status->pivot_args_count = argc;

    return status;
}

int pivot_run_main(PivotState *status) {
    if (status->pivot_args_count < 2) {
        printf("Expected argument: source file path");
        return 1;
    }

    char *source_path = status->pivot_args[1];
    LexerState *state = tokenize_file(source_path);

    if (state->error != LEXER_OK) {
        printf("Lexer Error: %s", out_error(state->error));
        return 1;
    }

    lexer_state_free(state);
    return 0;
}
