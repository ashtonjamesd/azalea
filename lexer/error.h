#ifndef LEXER_ERROR_H
#define LEXER_ERROR_H

struct LexerState;

typedef enum LexerError {
    LEXER_OK,
    LEXER_FILE_ERROR,
    LEXER_FILE_EXT_ERROR,
    LEXER_MEMORY_ERROR,
    LEXER_UNEXPECTED_CHARACTER_ERROR,
    LEXER_EXPECTED_CHARACTER,
} LexerError;

extern void set_error(struct LexerState *state, LexerError error);
extern char *out_error(LexerError error);

#endif