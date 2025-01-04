#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "symbol_table.h"

void set_variable(SymbolTable *table, const char *name, VariableType type, void *val) {
    VariableSymbol *existing_var = get_variable(table, name);
    if (existing_var != NULL) {
            if (type == VAR_TYPE_STR) {
            existing_var->as.str_val = strdup((char *)val);
        } 
        else if (type == VAR_TYPE_INT) {
            existing_var->as.int_val = *(int *)val;
        }
        return;
    }

    VariableSymbol *symbol = (VariableSymbol *)malloc(sizeof(VariableSymbol));
    symbol->name = strdup(name);
    symbol->type = type;
    
    if (type == VAR_TYPE_STR) {
        symbol->as.str_val = strdup((char *)val);
    } 
    else if (type == VAR_TYPE_INT) {
        symbol->as.int_val = *(int *)val;
    }

    if (table->count >= table->capacity) {
        table->capacity *= 2;
        table->symbols = realloc(table->symbols, sizeof(VariableSymbol) * table->capacity);
    }

    table->symbols[table->count++] = *symbol;
}

VariableSymbol *get_variable(SymbolTable *table, char *name) {
    for (int i = 0; i < table->count; i++) {
        if (strcmp(table->symbols[i].name, name) == 0) {
            return &table->symbols[i];
        }
    }

    return NULL;
}

SymbolTable *init_symbol_table() {
    SymbolTable *table = (SymbolTable *)malloc(sizeof(SymbolTable));
    table->capacity = 2;
    table->count = 0;
    table->symbols = (VariableSymbol *)malloc(sizeof(VariableSymbol) * table->capacity);

    return table;
}