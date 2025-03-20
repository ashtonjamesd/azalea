#include <stdio.h>
#include <stdlib.h>

#include "io.h"

void _azalea_print(void *args[]) {
    printf("%s", (char *)args[0]);
}

void _azalea_println(void *args[]) {
    printf("%s\n", (char *)args[0]);
}

char *_azalea_readln() {
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