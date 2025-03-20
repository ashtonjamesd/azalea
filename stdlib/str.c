#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "str.h"

char *_azalea_concat(void *args[]) {
    char *str1 = (char *)args[0];
    char *str2 = (char *)args[1];

    return strcat(str1, str2);
}

char *_azalea_from(void *args[]) {
    int *n = (int *)args[0];

    char *result = (char *)malloc(12 * sizeof(char));
    if (result == NULL) {
        perror("Failed to allocate memory");
        return NULL;
    }

    snprintf(result, 12, "%d", *n);
    return result;
}

char *_azalea_as_upper(void *args[]) {
    char *str = (char *)args[0];

    char *s = str;
    while (*s) {
        *s = toupper((unsigned char) *s);
        s++;
    }

    return str;
}

char *_azalea_as_lower(void *args[]) {
    char *str = (char *)args[0];

    char *s = str;
    while (*s) {
        *s = tolower((unsigned char) *s);
        s++;
    }

    return str;
}