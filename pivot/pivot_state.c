#include <stdio.h>
#include <stdlib.h>

#include "pivot_state.h"
#include "error.h"
#include "state.h"
#include "lexer.h"
#include "parser.h"

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
    
    LexerState *lexer_state = tokenize_file(source_path);
    if (lexer_state->error != LEXER_OK) {
        printf("Lexer Error: %s", out_error(lexer_state->error));
        return 1;
    }

    ParserState *state = parse_tokens(lexer_state->tokens);
    


    lexer_state_free(lexer_state);
    return 0;
}
