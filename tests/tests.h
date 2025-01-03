#include "token.h"

#ifndef TESTS_H
#define TESTS_H

extern void run_tests();
extern void run_lexer_tests();

extern void ASSERT_TOKEN_TYPE(LexerTokenType type, LexerTokenType expected);

extern int assertions_failed;
extern int tests_passed;
extern int tests_failed;

#endif