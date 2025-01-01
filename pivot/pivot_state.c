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
    tokenize_file(status->pivot_args[1]);

    return 0;
}
