#include <stdio.h>
#include <stdlib.h>

#include "pivot_state.h"
#include "lexer.h"

PivotState *init_pivot(int argc, char **argv) {
    PivotState *status = (PivotState *)malloc(sizeof(PivotState));
    status->exec_type = FILE_EXEC;
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

    if (state->statuscode != 0) {
        printf("Lexer Error: %s", state->error);
        return 1;
    }

    return 0;
}
