#ifndef FUNCTION_REGISTRY_H
#define FUNCTION_REGISTRY_H

typedef struct FunctionRegistryEntry {
    char *name;
    void *function_pointers;
} FunctionRegistryEntry;

extern void register_function(const char *name, void *function_pointer);
extern void *get_function(const char *name);

#endif