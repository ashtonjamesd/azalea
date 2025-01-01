#ifndef AST_H
#define AST_H

typedef enum ExpressionType {
    VARIABLE_DECLARATION,
    NUMERIC_LITERAL,
    STRING_LITERAL,
    FLOAT_LITERAL,
} ExpressionType;

typedef struct Expression Expression;

typedef struct FloatLiteralExpression {
    float *value;
} FloatLiteralExpression;

typedef struct StringLiteralExpression {
    char *value;
} StringLiteralExpression;

typedef struct NumericLiteralExpression {
    int value;
} NumericLiteralExpression;

typedef struct VariableDeclaration {
    char *identifier;
    Expression *expr;
} VariableDeclaration;

typedef struct Expression {
    ExpressionType type;

    union {
        NumericLiteralExpression num_expr;
        StringLiteralExpression str_expr;
        FloatLiteralExpression flt_expr;
        VariableDeclaration var_decl;
    } as;
} Expression;

typedef struct Ast {
    Expression *body;
    int expression_count;
    int expression_capacity;
} Ast;

#endif