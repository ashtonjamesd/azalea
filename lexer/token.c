#include <stdio.h>

#include "token.h"

const LexerTokenMap keyword_token_type_map[] = {
    {"let", TOKEN_LET},
    {"use", TOKEN_USE},
    {"i32", TOKEN_INT_TYPE},
    {"f32", TOKEN_FLOAT_TYPE},
    {"str", TOKEN_STRING_TYPE},
    {"char", TOKEN_CHAR_TYPE},
    {"bool", TOKEN_BOOL_TYPE},
    {"true", TOKEN_TRUE},
    {"false", TOKEN_FALSE},
    {NULL, -1},
};

const LexerTokenMap single_token_type_map[] = {
    {"=", TOKEN_SINGLE_EQUALS},
    {";", TOKEN_SEMI_COLON},
    {":", TOKEN_COLON},
    {"(", TOKEN_LEFT_PAREN},
    {")", TOKEN_RIGHT_PAREN},
    {",", TOKEN_COMMA},
    {".", TOKEN_DOT},
    {"\\", TOKEN_ESCAPE_CHAR},
    {NULL, -1},
};
