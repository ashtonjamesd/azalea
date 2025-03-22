#ifndef TOKEN_H
#define TOKEN_H

typedef enum LexerTokenType {
    TOKEN_LET,
    TOKEN_USE,
    TOKEN_DEF,
    TOKEN_END,

    TOKEN_INT_TYPE,
    TOKEN_CHAR_TYPE,
    TOKEN_FLOAT_TYPE,
    TOKEN_BOOL_TYPE,
    TOKEN_STRING_TYPE,

    TOKEN_IDENTIFIER,

    TOKEN_SINGLE_EQUALS,
    TOKEN_SEMI_COLON,
    TOKEN_COLON,
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_COMMA,
    TOKEN_DOT,
    TOKEN_ESCAPE_CHAR,

    TOKEN_NUMERIC,
    TOKEN_STRING,
    TOKEN_FLOAT,
    TOKEN_CHAR,
    TOKEN_TRUE,
    TOKEN_FALSE,

    TOKEN_EOF
} LexerTokenType;

typedef struct LexerToken {
    char *lexeme;
    int line;
    LexerTokenType type;
} LexerToken;

typedef struct LexerTokenMap {
    char* lexeme;
    LexerTokenType type;
} LexerTokenMap;

extern const LexerTokenMap keyword_token_type_map[];
extern const LexerTokenMap single_token_type_map[];

#endif