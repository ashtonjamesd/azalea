#include <stdio.h>
#include <stdlib.h>

#include "tests.h"
#include "azalea_state.h"

int pivot_main(int argc, char **argv) {
    run_tests();

    int debug = 1;
    AzaleaState *azalea = init_azalea(argc, argv, debug);
    if (azalea == NULL) {
        printf("Unable to initialise azalea interpreter");
        return 1;
    }

    return azalea_run_main(azalea);
}

int main(int argc, char **argv) {
    return pivot_main(argc, argv);
}