#include <stdio.h>
#include <stdlib.h>

#include "pivot_state.h"
#include "state.h"
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "function_registry.h"
#include "interpreter.h"

PivotState *init_pivot(int argc, char **argv) {
    PivotState *status = (PivotState *)malloc(sizeof(PivotState));
    if (!status) {
        perror("Error allocating memory for interpreter state");
        return NULL;
    }

    status->pivot_args = argv;
    status->pivot_args_count = argc;
    status->debug_mode = 1;

    return status;
}

int pivot_run_main(PivotState *state) {
    if (state->pivot_args_count < 2) {
        printf("Expected argument: source file path");
        return 1;
    }

    initialise_registry();

    char *source_path = state->pivot_args[1];
    LexerState *lexer_state = init_lexer_state();
    lexer_state->print_debug = state->debug_mode;

    tokenize_file(lexer_state, source_path);
    if (lexer_state->error != NULL) {
        printf("\n%s\n\n", lexer_state->error);
        return 1;
    }

    ParserState *parser_state = init_parser(lexer_state->tokens);
    parser_state->debug_mode = state->debug_mode;

    parse_tokens(parser_state);
    if (parser_state->error != NULL) {
        printf("%s\n\n", parser_state->error);
        return 1;
    }

    printf("\n\n");
    interpret_ast(parser_state->ast);
    printf("\n");
    
    // free interpreter
    parser_state_free(parser_state);
    lexer_state_free(lexer_state);
    return 0;
}
