#ifndef PIVOT_STATUS_H
#define PIVOT_STATUS_H

typedef struct AzaleaState {
    int azalea_args_count;
    char **azalea_args;

    int debug_mode;
} AzaleaState;

extern AzaleaState *init_azalea(int argc, char **argv, int debug);
extern int azalea_run_main(AzaleaState *status);

#endif