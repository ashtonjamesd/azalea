#include "symbol_table.h"

#ifndef FUNCTION_REGISTRY_H
#define FUNCTION_REGISTRY_H

#define MAX_FUNCTIONS 100
#define MAX_MODULES 10

typedef void *(*FunctionPointer)(void *args);

typedef struct FunctionRegistryEntry {
    char *name;
    FunctionPointer func;
    int param_count;
    VariableType *param_types;
    VariableType return_type;
} FunctionRegistryEntry;

typedef struct FunctionRegistryModule {
    char *name;
    FunctionRegistryEntry functions[MAX_FUNCTIONS];
    int function_count;
} FunctionRegistryModule;

extern void register_function(char *module, char *name, void *func, int param_count, VariableType *param_types, VariableType return_type);
extern FunctionRegistryEntry *get_function(char *module, const char *name);
extern void initialise_registry();

#endif