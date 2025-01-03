#include <stdio.h>
#include <stdlib.h>

#include "pivot_state.h"
#include "state.h"
#include "lexer.h"
#include "parser.h"
#include "ast.h"

PivotState *init_pivot(int argc, char **argv) {
    PivotState *status = (PivotState *)malloc(sizeof(PivotState));
    if (!status) {
        perror("Error allocating memory for interpreter state");
        return NULL;
    }

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
    
    LexerState *lexer_state = init_lexer_state();
    tokenize_file(lexer_state, source_path);
    if (lexer_state->error != NULL) {
        printf("%s", lexer_state->error);
        return 1;
    }

    return 0;

    ParserState *parser_state = parse_tokens(lexer_state->tokens);
    if (parser_state->error != NULL) {
        printf("%s", parser_state->error);
        return 1;

    }
    
    parser_state_free(parser_state);
    lexer_state_free(lexer_state);
    return 0;
}
