#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "function_registry.h"
#include "io.h"
#include "math.h"

static int module_count = 0;

static FunctionRegistryModule modules[MAX_MODULES];

FunctionRegistryModule *get_module(char *name) {
    for (int i = 0; i < module_count; i++) {
        if (strcmp(modules[i].name, name) == 0) {
            return &modules[i];
        }
    }

    return NULL;
}

void register_module(char *name) {
    for (int i = 0; i < module_count; i++) {
        if (strcmp(modules[i].name, name) == 0) {
            return;
        }
    }

    if (module_count < MAX_MODULES) {
        modules[module_count].name = strdup(name);
        modules[module_count].function_count = 0;
        module_count++;
    } else {
        printf("Maximum module limit reached!\n");
    }
}

void register_function(char *module, char *name, void *func, int param_count, VariableType *param_types, VariableType return_type) {
    FunctionRegistryModule *module_entry = NULL;

    for (int i = 0; i < module_count; i++) {
        if (strcmp(modules[i].name, module) == 0) {
            module_entry = &modules[i];
            break;
        }
    }

    if (!module_entry) {
        register_module(module);
        module_entry = &modules[module_count++];
    }

    FunctionRegistryEntry *function_entry = &module_entry->functions[module_entry->function_count];
    function_entry->name = strdup(name);
    function_entry->func = (FunctionPointer)func;
    function_entry->param_count = param_count;
    function_entry->return_type = return_type;

    function_entry->param_types = (VariableType *)malloc(param_count * sizeof(VariableType));
    memcpy(function_entry->param_types, param_types, param_count * sizeof(VariableType));

    module_entry->function_count++;
}

FunctionRegistryEntry *get_function(char *module, const char *name) {
    if (module == NULL) return NULL;

    for (int i = 0; i < module_count; i++) {
        if (strcmp(modules[i].name, module) == 0) {
            for (int j = 0; j < modules[i].function_count; j++) {
                if (strcmp(modules[i].functions[j].name, name) == 0) {
                    return &modules[i].functions[j];
                }
            }
        }
    }

    return NULL;
}

void print_registry() {
    printf("\n\n");
    for (int i = 0; i < module_count; i++) {
        printf("Registered Module: '%s' with %d functions", modules[i].name, modules[i].function_count);
    }

    printf("\n\n");
    for (int i = 0; i < module_count; i++) {
        printf("%s:\n", modules[i].name);

        for (int j = 0; j < modules[i].function_count; j++) {
            printf("  Func: %s\n", modules[i].functions[j].name);

            for (int k = 0; k < modules[i].functions[j].param_count; k++) {
                printf("    Param: %d\n", modules[i].functions[j].param_types[k]);
            }
        }
    }
    printf("\n");
}

void initialise_std() {
    register_module("std");
    
    VariableType println_types[] = { VAR_TYPE_STR  };
    register_function("std", "println", &_pivot_println, 1, println_types, VAR_TYPE_NULL);

    VariableType print_types[] = { VAR_TYPE_STR  };
    register_function("std", "print", &_pivot_print, 1, print_types, VAR_TYPE_NULL);

    VariableType readln_types[] = { };
    register_function("std", "readln", &_pivot_readln, 0, readln_types, VAR_TYPE_STR);
}

void initialise_math() {
    register_module("math");
    
    VariableType exp_types[] = { VAR_TYPE_INT, VAR_TYPE_INT  };
    register_function("math", "exp", &_pivot_exp, 2, exp_types, VAR_TYPE_INT);
}

void initialise_registry() {
    initialise_std();
    initialise_math();
    
    print_registry();
}