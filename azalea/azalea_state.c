#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "function_registry.h"
#include "interpreter.h"
#include "azalea_state.h"

AzaleaState *init_azalea(int argc, char **argv, int debug) {
    AzaleaState *status = (AzaleaState *)malloc(sizeof(AzaleaState));
    if (!status) {
        perror("Error allocating memory for interpreter state");
        return NULL;
    }

    status->azalea_args = argv;
    status->azalea_args_count = argc;
    status->debug_mode = debug;

    return status;
}

int azalea_run_main(AzaleaState *state) {
    if (state->azalea_args_count < 2) {
        printf("Expected argument: source file path");
        return 1;
    }

    initialise_registry(state->debug_mode);

    char *source_path = state->azalea_args[1];
    LexerState *lexer_state = init_lexer_state();
    lexer_state->debug = state->debug_mode;

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

    interpret_ast(parser_state->ast);
    
    // free interpreter
    parser_state_free(parser_state);
    lexer_state_free(lexer_state);
    return 0;
}
