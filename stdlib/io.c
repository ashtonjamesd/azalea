#include <stdio.h>
#include <stdlib.h>

#include "io.h"

void _pivot_print(char *str) {
    printf("%s", str);
}

void _pivot_println(char *str) {
    printf("%s\n", str);
}

char *_pivot_readln() {
    size_t buffer_size = 1;
    size_t input_length = 0;
    char *buffer = (char *)malloc(buffer_size);

    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        if (input_length + 1 >= buffer_size) {
            buffer_size *= 2;
            char *new_buff = (char*)realloc(buffer, buffer_size);
            buffer = new_buff;
        }

        buffer[input_length++] = (char)ch;
    }

    buffer[input_length] = '\0';
    
    return buffer;
}