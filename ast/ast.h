#ifndef AST_H
#define AST_H

typedef enum ExpressionType {
    VARIABLE_DECLARATION,
    NUMERIC_LITERAL,
    IDENTIFIER,
    STRING_LITERAL,
    FLOAT_LITERAL,
    CHAR_LITERAL
} ExpressionType;

typedef struct Expression Expression;

typedef struct IdentifierExpression {
    char *identifier;
} IdentifierExpression;

typedef struct CharLiteralExpression {
    char value;
} CharLiteralExpression;

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
        CharLiteralExpression char_expr;
        FloatLiteralExpression flt_expr;
        VariableDeclaration var_decl;
        IdentifierExpression ident_expr;
    } as;
} Expression;

typedef struct Ast {
    Expression *body;
    int expression_count;
    int expression_capacity;
} Ast;

typedef struct ParserState ParserState;

extern void print_ast_body(Ast *ast);

#endif