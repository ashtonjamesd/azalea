#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "function_registry.h"
#include "io.h"

#define MAX_FUNCTIONS 100
static int function_count;

static FunctionRegistryEntry registry[MAX_FUNCTIONS];

void register_function(char *name, void *func) {
    registry[function_count].name = name;
    registry[function_count].func = func;

    function_count++;
}

FunctionPointer get_function(const char *name) {
    for (int i = 0; i < function_count; i++) {
        if (strcmp(registry[i].name, name) == 0) {
            return registry[i].func;
        }
    }

    return NULL;
}

void initialise_registry() {
    register_function("print", &_pivot_print);
}