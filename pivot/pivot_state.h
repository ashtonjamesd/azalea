#ifndef PIVOT_STATUS_H
#define PIVOT_STATUS_H

typedef enum PivotStatus {
    PIVOT_STATUS_OK,
    PIVOT_STATUS_ERR
} PivotStatus;

typedef struct PivotState {
    PivotStatus status_code;
    int pivot_args_count;
    char **pivot_args;
} PivotState;

extern PivotState *init_pivot(int argc, char **argv);
extern int pivot_run_main(PivotState *status);

#endif