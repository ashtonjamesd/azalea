#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int *_azalea_exp(void *args[]) {
    int *total = (int *)malloc(sizeof(int));

    *total = 1;

    int n = *(int *)args[0];
    int exp = *(int *)args[1];

    for (int i = 0; i < exp; i++) {
        *total *= n;
    }

    return total;
}

int *_azalea_max(void *args[]) {
    int *n1 = (int *)args[0];
    int *n2 = (int *)args[1];

    if (*n1 > *n2) return n1;
    return n2; 
}

int *_azalea_min(void *args[]) {
    int *n1 = (int *)args[0];
    int *n2 = (int *)args[1];

    if (*n1 > *n2) return n2;
    return n1;
}