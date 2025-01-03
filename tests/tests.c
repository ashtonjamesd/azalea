#include <stdio.h>

#include "tests.h"

int tests_passed = 0;
int tests_failed = 0;

int assertions_failed = 0;

inline void ASSERT_TOKEN_TYPE(LexerTokenType type1, LexerTokenType type2) {
    if (type1 != type2) {
        printf("ASSERTION FAILED: %d != %d\n", type1, type2);
        assertions_failed++;
    }
}

void run_tests() {
    run_lexer_tests();

    printf("\nRan %d tests\n", tests_failed + tests_passed);
    printf("  Passed: %d\n", tests_passed);
    printf("  Failed: %d\n\n", tests_failed);
}