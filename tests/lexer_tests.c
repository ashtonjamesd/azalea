#include <stdio.h>

#include "state.h"
#include "lexer.h"

static int tests_passed = 0;
static int tests_failed = 0;

static int assertions_failed = 0;

static inline void ASSERT_TOKEN_TYPE(LexerTokenType type1, LexerTokenType type2) {
    if (type1 != type2) {
        printf("ASSERTION FAILED: %d != %d\n", type1, type2);
        assertions_failed++;
    }
}

static void test_variable_declaration() {
    LexerState *lexer_state = init_lexer_state();
    lexer_state->print_debug = 0;
    
    tokenize_file(lexer_state, "tests\\samples\\variable_declaration.pv");
    
    ASSERT_TOKEN_TYPE(lexer_state->tokens[0].type, TOKEN_LET);
    ASSERT_TOKEN_TYPE(lexer_state->tokens[1].type, TOKEN_IDENTIFIER);
    ASSERT_TOKEN_TYPE(lexer_state->tokens[2].type, TOKEN_SINGLE_EQUALS);
    ASSERT_TOKEN_TYPE(lexer_state->tokens[3].type, TOKEN_STRING);
    ASSERT_TOKEN_TYPE(lexer_state->tokens[4].type, TOKEN_SEMI_COLON);

    lexer_state_free(lexer_state);
}

static void test_numeric_literal() {
    LexerState *lexer_state = init_lexer_state();
    lexer_state->print_debug = 0;
    
    tokenize_file(lexer_state, "tests\\samples\\numeric_literal.pv");
    
    ASSERT_TOKEN_TYPE(lexer_state->tokens[0].type, TOKEN_NUMERIC);
    ASSERT_TOKEN_TYPE(lexer_state->tokens[1].type, TOKEN_SEMI_COLON);

    lexer_state_free(lexer_state);
}

static void test_string_literal() {
    LexerState *lexer_state = init_lexer_state();
    lexer_state->print_debug = 0;
    
    tokenize_file(lexer_state, "tests\\samples\\string_literal.pv");
    
    ASSERT_TOKEN_TYPE(lexer_state->tokens[0].type, TOKEN_STRING);
    ASSERT_TOKEN_TYPE(lexer_state->tokens[1].type, TOKEN_SEMI_COLON);

    lexer_state_free(lexer_state);
}

static void test_boolean_literal() {
    LexerState *lexer_state = init_lexer_state();
    lexer_state->print_debug = 0;
    
    tokenize_file(lexer_state, "tests\\samples\\boolean_literal.pv");
    
    ASSERT_TOKEN_TYPE(lexer_state->tokens[0].type, TOKEN_TRUE);
    ASSERT_TOKEN_TYPE(lexer_state->tokens[1].type, TOKEN_SEMI_COLON);

    lexer_state_free(lexer_state);
}

static void test_char_literal() {
    LexerState *lexer_state = init_lexer_state();
    lexer_state->print_debug = 0;
    
    tokenize_file(lexer_state, "tests\\samples\\char_literal.pv");
    
    ASSERT_TOKEN_TYPE(lexer_state->tokens[0].type, TOKEN_CHAR);
    ASSERT_TOKEN_TYPE(lexer_state->tokens[1].type, TOKEN_SEMI_COLON);

    lexer_state_free(lexer_state);
}

static void run_test(void *function, char* function_name) {
    int initial_tests_failed = assertions_failed;

    void (*test_func)() = function;
    test_func();

    if (assertions_failed == initial_tests_failed) {
        tests_passed++;
    } else {
        printf("TEST FAILED: '%s'\n", function_name);
        tests_failed++;
    }
}

static void run_lexer_tests() {
    run_test(&test_variable_declaration, "test_variable_declaration");
    run_test(&test_numeric_literal, "test_numeric_literal");
    run_test(&test_string_literal, "test_string_literal");
    run_test(&test_boolean_literal, "test_boolean_literal");
    run_test(&test_char_literal, "test_char_literal");
}

void run_tests() {
    run_lexer_tests();

    printf("\nRan %d tests\n", tests_failed + tests_passed);
    printf("  Passed: %d\n", tests_passed);
    printf("  Failed: %d\n\n", tests_failed);
}