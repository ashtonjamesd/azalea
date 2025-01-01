#include <stdio.h>
#include <stdlib.h>
#include "pivot_state.h"

int pivot_main(int argc, char **argv) {
    PivotState *pivot = init_pivot(argc, argv);
    if (pivot == NULL) {
        return 1;
    }

    return pivot_run_main(pivot);
}

int main(int argc, char **argv) {
    return pivot_main(argc, argv);
}