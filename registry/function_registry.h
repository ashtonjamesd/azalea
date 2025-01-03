#ifndef FUNCTION_REGISTRY_H
#define FUNCTION_REGISTRY_H

typedef void (*FunctionPointer)(void *args);

typedef struct FunctionRegistryEntry {
    char *name;
    FunctionPointer func;
} FunctionRegistryEntry;

extern void register_function(char *name, void *func); // does this need to be here
extern FunctionPointer get_function(const char *name);
extern void initialise_registry();

#endif