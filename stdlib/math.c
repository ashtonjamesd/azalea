#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int *_pivot_exp(void *args[]) {
    int *total = (int *)malloc(sizeof(int));

    *total = 1;

    int n = *(int *)args[0];
    int exp = *(int *)args[1];

    for (int i = 0; i < exp; i++) {
        *total *= n;
    }

    return total;
}
