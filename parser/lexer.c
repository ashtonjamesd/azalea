#include <stdio.h>
#include <stdlib.h>

#include "state.h"

int tokenize_file(char *path) {
    FILE *fptr = fopen(path, "r");
    if (!fptr) {
        perror("Error opening file");
        return 1;
    }

    fseek(fptr, 0, SEEK_END);
    long sz = ftell(fptr);
    rewind(fptr);

    char *buff = (char *)malloc(sz);
    if (!buff) {
        perror("Error allocating memory");
        fclose(fptr);
        return 1;
    }

    fread(buff, 1, sz, fptr);
    buff[sz] = '\0';

    printf("%s", buff);


    fclose(fptr);
    free(buff);
}