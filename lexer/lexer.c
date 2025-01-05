#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "token.h"
#include "lexer.h"

LexerState *init_lexer_state() {
    LexerState *state = (LexerState *)malloc(sizeof(LexerState));
    state->tokens = (LexerToken *)malloc(sizeof(LexerToken) * 2);
    state->current = 0;
    state->error = NULL;
    state->token_capacity = 2;
    state->token_count = 0;
    state->line = 1;
    state->print_debug = 1;

    return state;
}

static void raise_lexer_error(LexerState *state, char *error) {
    state->error = error;
}

static void raise_defect(LexerState *state, char *error) {
    char *formatted_error = (char *)malloc(strlen(error));
    sprintf(formatted_error, "%s on line %d", error, state->line);
    state->error = formatted_error;
    free(formatted_error);
}

static void read_source(LexerState *state, char *path) {
    FILE *fptr = fopen(path, "r");
    if (!fptr) {
        raise_lexer_error(state, "Unable to open source file");
        return;
    }

    char *file_ext = strrchr(path, '.');
    if (strcmp(file_ext, ".pv") != 0) {
        raise_lexer_error(state, "Source file must be a valid pivot (.pv) file");
        return;
    }

    fseek(fptr, 0, SEEK_END);
    long sz = ftell(fptr);
    rewind(fptr);

    char *buff = (char *)malloc(sz + 1);
    if (!buff) {
        raise_lexer_error(state, "Error allocating memory for source file");
        fclose(fptr);
        return;
    }

    size_t read_size = fread(buff, 1, sz, fptr);
    buff[sz] = '\0';

    if (read_size != sz) {
        printf("Warning: Read size (%zu) does not match expected size (%ld). Cutting off extra characters.\n", read_size, sz);
        buff[read_size] = '\0'; 
    }

    state->source = strdup(buff);
    
    fclose(fptr);
    free(buff);
}

static void create_token(LexerState *state, char *lexeme, LexerTokenType type) {
    LexerToken *token = (LexerToken *)malloc(sizeof(LexerToken));
    token->lexeme = lexeme;
    token->type = type;
    token->line = state->line;

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
                raise_defect(state, "Invalid numeric literal");
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

    if (has_decimal) {
        create_token(state, lexeme, TOKEN_FLOAT);
    } else {
        create_token(state, lexeme, TOKEN_NUMERIC);
    }

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
    advance(state);

    int buff_size = 64;
    int buff_idx = 0;
    char *buffer = (char *)malloc(buff_size);

    while (!is_end(state) && get_current(state) != '\"') {
        char c = get_current(state);

        if ('\\' == c) {
            advance(state);
            if (is_end(state)) {
                raise_defect(state, "unterminated escape sequence");
                free(buffer);
                return;
            }

            c = get_current(state);
            switch (c) {
                case 'n': c = '\n'; break;
                case 't': c = '\t'; break;
                case 'r': c = '\r'; break;
                case '\"': c = '\"'; break;
                case '\\': c = '\\'; break;
                default:
                    raise_defect(state, "invalid escape sequence");
                    free(buffer);
                    return;
            }
        }

        if (buff_idx >= buff_size) {
            buff_size *= 2;
            char *new_buff = (char *)realloc(buffer, buff_size);
            buffer = new_buff;
        }

        buffer[buff_idx++] = c;

        advance(state);
    }

    if (is_end(state) || get_current(state) != '\"') {
        raise_defect(state, "unterminated string literal");
        free(buffer);
        return;
    }

    advance(state);
    buffer[buff_idx] = '\0';

    state->current--;
    create_token(state, buffer, TOKEN_STRING);
    free(buffer);
}

static void parse_char(LexerState *state) {
    advance(state);

    char lexeme[2] = {get_current(state), '\0'};
    create_token(state, strdup(lexeme), TOKEN_CHAR);

    if (lexeme[0] == '\'') {
        raise_defect(state, "empty char literal");
        return;
    }

    advance(state);
    if (get_current(state) != '\'') {
        raise_defect(state, "unterminated char literal");
        return;
    }
}

static void next_token(LexerState *state) {
    char current_token = get_current(state);

    if (check_symbols(state, current_token)) {
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

void print_lexer(LexerState *state) {
    printf("\nTokens:\n");
    for (int i = 0; i < state->token_count; i++) {
        printf(" L:%d  Token: %s | %d\n", state->tokens[i].line, state->tokens[i].lexeme, state->tokens[i].type);
    }
}

void tokenize_file(LexerState *state, char *path) {
    read_source(state, path);
    if (state->error != NULL) {
        printf("%s\n\n", state->error);
        return;
    }

    while (get_current(state) != '\0') {
        while (get_current(state) == ' ' || get_current(state) == '\t') {
            advance(state);
        }

        if (get_current(state) == '\n') {
            state->line++;
        }

        next_token(state);
        if (get_current(state) == '\0') break;
        
        if (state->error != NULL) {
            return;
        }

        advance(state);
    }

    create_token(state, "eof", TOKEN_EOF);

    if (state->print_debug) print_lexer(state);
}

void lexer_state_free(LexerState *state) {
    free(state->source);
    free(state);
}