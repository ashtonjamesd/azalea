#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

typedef enum VariableType {
    VAR_TYPE_STR,
    VAR_TYPE_INT,
    VAR_TYPE_BOOL,
    VAR_TYPE_CHAR,
    VAR_TYPE_FLOAT,
    VAR_TYPE_NULL
} VariableType;

typedef struct VariableSymbol {
    VariableType type;
    char *name;
    int is_mutable;

    union {
        char *str_val;
        int *int_val;
    } as;

} VariableSymbol;

typedef struct SymbolTable {
    VariableSymbol *symbols;
    int count;
    int capacity;
} SymbolTable;

extern void set_variable(SymbolTable *symbols, char *name, VariableType type, void* val);
extern VariableSymbol *get_variable(SymbolTable *symbols, char *name);
extern SymbolTable *init_symbol_table();

#endif