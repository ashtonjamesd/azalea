#ifndef PIVOT_STATUS_H
#define PIVOT_STATUS_H


typedef struct PivotState {
    int pivot_args_count;
    char **pivot_args;

    int debug_mode;
} PivotState;

extern PivotState *init_pivot(int argc, char **argv);
extern int pivot_run_main(PivotState *status);

#endif