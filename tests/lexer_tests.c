#include <stdio.h>

#include "state.h"
#include "lexer.h"

static int tests_passed = 0;
static int tests_failed = 0;

static inline void ASSERT_TOKEN_TYPE(LexerTokenType type1, LexerTokenType type2) {
    if (type1 != type2) {
        printf("ASSERTION FAILED: %d != %d\n", type1, type2);
        tests_failed++;
    }  else {
        tests_passed++;
    }
}

static void test_variable_declaration() {
    LexerState *lexer_state = init_lexer_state();
    lexer_state->print_debug = 0;
    
    tokenize_file(lexer_state, "tests\\samples\\variable_declaration.pv");
    
    ASSERT_TOKEN_TYPE(lexer_state->tokens[0].type, TOKEN_LET);
    ASSERT_TOKEN_TYPE(lexer_state->tokens[1].type, TOKEN_IDENTIFIER);
    ASSERT_TOKEN_TYPE(lexer_state->tokens[2].type, TOKEN_SINGLE_EQUALS);
    ASSERT_TOKEN_TYPE(lexer_state->tokens[3].type, TOKEN_NUMERIC);
    ASSERT_TOKEN_TYPE(lexer_state->tokens[4].type, TOKEN_SEMI_COLON);

    lexer_state_free(lexer_state);
}

static void run_test(void *function) {
    void (*test_func)() = function;
    test_func();
}

void run_tests() {
    run_test(&test_variable_declaration);

    printf("Ran %d tests", tests_failed + tests_passed);
    printf("Passed: %d\n", tests_passed);
    printf("Failed: %d\n", tests_failed);
}