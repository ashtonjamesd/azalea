#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "function_registry.h"
#include "io.h"

static int module_count = 0;

static FunctionRegistryModule modules[MAX_MODULES];

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

void register_function(char *module, char *name, void *func) {
    FunctionRegistryModule *module_entry = NULL;
    
    for (int i = 0; i < module_count; i++) {
        if (strcmp(modules[i].name, module) == 0) {
            module_entry = &modules[i];
            break;
        }
    }

    if (!module) {
        register_module(module);
        module_entry = &modules[module_count++];
    }


    module_entry->functions[module_entry->function_count].name = strdup(name);
    module_entry->functions[module_entry->function_count].func = (FunctionPointer)func;
    module_entry->function_count++;
}

FunctionPointer get_function(char *module, const char *name) {
    if (module == NULL) return NULL;

    for (int i = 0; i < module_count; i++) {
        if (strcmp(modules[i].name, module) == 0) {
            for (int j = 0; j < modules[i].function_count; j++) {
                if (strcmp(modules[i].functions[j].name, name) == 0) {
                    return modules[i].functions[j].func;
                }
            }
        }
    }

    return NULL;
}

void initialise_registry() {
    register_module("std");

    register_function("std", "print", &_pivot_print);
    register_function("std", "println", &_pivot_println);

    printf("\n\n");
    for (int i = 0; i < module_count; i++) {
        printf("Registered Module: '%s' with %d functions", modules[i].name, modules[i].function_count);
    }

    printf("\n\n");
    for (int i = 0; i < module_count; i++) {
        printf("%s:\n", modules[i].name);

        for (int j = 0; j < modules[j].function_count; j++) {
            printf("  Func: %s\n", modules[i].functions[j].name);
        }
    }
}