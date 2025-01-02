#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "state.h"
#include "error.h"
#include "token.h"

static LexerState *init_lexer_state() {
    LexerState *state = (LexerState *)malloc(sizeof(LexerState));
    state->tokens = (LexerToken *)malloc(sizeof(LexerToken) * 2);
    state->current = 0;
    state->error = LEXER_OK;
    state->token_capacity = 2;
    state->token_count = 0;

    return state;
}

static void read_source(LexerState *state, char *path) {
    FILE *fptr = fopen(path, "r");
    if (!fptr) {
        set_error(state, LEXER_FILE_ERROR);
        return;
    }

    char *file_ext = strrchr(path, '.');
    if (strcmp(file_ext, ".pv") != 0) {
        set_error(state, LEXER_FILE_EXT_ERROR);
        return;
    }

    fseek(fptr, 0, SEEK_END);
    long sz = ftell(fptr);
    rewind(fptr);

    char *buff = (char *)malloc(sz);
    if (!buff) {
        set_error(state, LEXER_MEMORY_ERROR);
        fclose(fptr);
        return;
    }

    fread(buff, 1, sz, fptr);
    buff[sz] = '\0';

    state->source = strdup(buff);
    
    fclose(fptr);
    free(buff);
}

static void create_token(LexerState *state, char *lexeme, LexerTokenType type) {
    LexerToken *token = (LexerToken *)malloc(sizeof(LexerToken));
    token->lexeme = lexeme;
    token->type = type;

    if (state->token_count >= state->token_capacity) {
        state->token_capacity *= 2;
        state->tokens = realloc(state->tokens, sizeof(LexerToken) * state->token_capacity);
    }

    state->tokens[state->token_count++] = *token;
}

static int is_end(LexerState *state) {
    if (state->source[state->current] == '\0') {
        return 1;
    }

    return 0;
}

static inline char get_current(LexerState *state) {
    return state->source[state->current];
}

static inline void advance(LexerState *state) {
    state->current++;
}

static void parse_numeric(LexerState *state) {
    int start = state->current;

    int has_decimal = 0;
    while (!is_end(state) && (isdigit(get_current(state)) || get_current(state) == '.')) {
        if (get_current(state) == '.') {
            if (has_decimal) {
                set_error(state, LEXER_UNEXPECTED_CHARACTER_ERROR);
                return;
            }

            has_decimal = 1;
        }

        advance(state);
    }

    int len = (state->current - start);

    char *lexeme = (char *)malloc(len + 1);
    strncpy(lexeme, state->source + start, len);
    
    lexeme[len] = '\0';

    create_token(state, lexeme, TOKEN_NUMERIC);
    state->current--;
}

static void parse_identifier(LexerState *state) {
    int start = state->current;

    while (!is_end(state) && isalnum(get_current(state))) {
        advance(state);
    }

    int len = (state->current - start);

    char *lexeme = (char *)malloc(len + 1);
    strncpy(lexeme, state->source + start, len);
    
    lexeme[len] = '\0';

    LexerTokenType type = TOKEN_IDENTIFIER;
    for (size_t i = 0; keyword_token_type_map[i].lexeme != NULL; i++) {
        if (strcmp(keyword_token_type_map[i].lexeme, lexeme) == 0) {
            type = keyword_token_type_map[i].type;
            break;
        }
    }

    create_token(state, lexeme, type);
    state->current--;
}

static int check_symbols(LexerState *state, char current_token) {
    for (size_t i = 0; single_token_type_map[i].lexeme != NULL; i++) {
        if (single_token_type_map[i].lexeme[0] == current_token) {
            create_token(state, strdup(single_token_type_map[i].lexeme), single_token_type_map[i].type);
            return 1;
        }
    }
    
    return 0;
}

static void parse_string(LexerState *state) {
    int start = state->current;

    advance(state);
    while (!is_end(state) && get_current(state) != '\"') {
        advance(state);
    }
    advance(state);

    int len = (state->current - start);

    char *lexeme = (char *)malloc(len + 1);
    strncpy(lexeme, state->source + start, len);
    
    lexeme[len] = '\0';

    create_token(state, lexeme, TOKEN_STRING);
    state->current--;
}

static void parse_char(LexerState *state) {
    advance(state);
    char lexeme[2] = {get_current(state), '\0'};
    create_token(state, strdup(lexeme), TOKEN_CHAR);

    advance(state);
    if (get_current(state) != '\'') {
        set_error(state, LEXER_EXPECTED_CHARACTER);
        return;
    }

    advance(state);
}

static void next_token(LexerState *state) {
    char current_token = get_current(state);

    if (check_symbols(state, current_token)) {
        advance(state);
        return;
    }

    if (isdigit(current_token)) {
        parse_numeric(state);
    }
    else if (isalpha(current_token)) {
        parse_identifier(state);
    }
    else if (current_token == '\"') {
        parse_string(state);
    }
    else if (current_token == '\'') {
        parse_char(state);
    }
}

LexerState *tokenize_file(char *path) {
    LexerState *state = init_lexer_state();
    
    read_source(state, path);
    if (state->error != LEXER_OK) {
        return state;
    }

    while (get_current(state) != '\0') {
        while (get_current(state) == ' ') {
            advance(state);
        }

        next_token(state);
        if (get_current(state) == '\0') break;
        
        if (state->error != LEXER_OK) {
            return state;
        }

        advance(state);
    }

    create_token(state, "eof", TOKEN_EOF);

    for (int i = 0; i < state->token_count; i++) {
        printf("Token: %s | %d\n", state->tokens[i].lexeme, state->tokens[i].type);
    }

    return state;
}