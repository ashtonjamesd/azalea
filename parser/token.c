#include <stdio.h>

#include "token.h"

const LexerTokenMap keyword_token_type_map[] = {
    {"let", TOKEN_LET},
    {NULL, -1},
};

const LexerTokenMap single_token_type_map[] = {
    {"=", TOKEN_SINGLE_EQUALS},
    {";", TOKEN_SEMI_COLON},
    {NULL, -1},
};
