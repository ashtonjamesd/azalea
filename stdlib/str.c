#include <string.h>

#include "str.h"

char *_pivot_concat(void *args[]) {
    char *str1 = (char *)args[0];
    char *str2 = (char *)args[1];

    return strcat(str1, str2);
}