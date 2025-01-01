#ifndef TOKEN_H
#define TOKEN_H

typedef enum LexerTokenType {
    TOKEN_LET,

    TOKEN_SINGLE_EQUALS,
    TOKEN_SEMI_COLON,

    TOKEN_IDENTIFIER,
    TOKEN_NUMERIC,
    TOKEN_EOF
} LexerTokenType;

typedef struct LexerToken {
    char *lexeme;
    LexerTokenType type;
} LexerToken;

typedef struct LexerTokenMap {
    char* lexeme;
    LexerTokenType type;
} LexerTokenMap;

extern const LexerTokenMap keyword_token_type_map[];
extern const LexerTokenMap single_token_type_map[];

#endif