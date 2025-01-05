#include <stdio.h>

#include "io.h"

void _pivot_print(char *str) {
    printf("%s", str);
}

void _pivot_println(char *str) {
    printf("%s\n", str);
}

char *_pivot_readln() {
    printf("reading line");
    return "test";
}