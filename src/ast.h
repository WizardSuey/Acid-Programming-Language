#ifndef ACID_AST_H
#define ACID_AST_H

typedef enum {
    AST_UNARY_OP_MINUS,    /* -x */
    AST_UNARY_OP_NOT       /* !x */
} AstUnaryOp;

typedef enum {
    AST_BINARY_OP_MUL,
    AST_BINARY_OP_DIV,
    AST_BINARY_OP_ADD,

    AST_BINARY_OP_ADD,
    AST_BINARY_OP_SUB,

    AST_BINARY_OP_LESS,
    AST_BINARY_OP_LESS_EQUAL,
    AST_BINARY_OP_GREATER,
    AST_BINARY_OP_GREATER_EQUAL,

    AST_BINARY_OP_EQUAL,
    AST_BINARY_OP_NOT_EQUAL,

    AST_BINARY_OP_AND,          /* x and y */
    AST_BINARY_OP_OR ,          /* x or y */

    AST_BINARY_OP_ASSIGNMENT    /* x = y */
} AstBinaryOp;

//* Типы узлов AST
typedef enum {
    AST_GLOBAL_VAR      = 143,
    AST_LOCAL_VAR       = 144,
    AST_FUNC_DECL       = 145,
    AST_DECL            = 146,
    AST_STRING          = 147,
    AST_FUNCALL         = 148,
    AST_IF              = 149,
    AST_WHILE           = 150,
    AST_RETURN          = 151,
    AST_FOR             = 152,
    AST_BLOCK           = 153,
} AstKind;

typedef struct {
    AstKind kind;
    union {
        /* Basic value */
        struct {
            double value;
        };
        /* String */
        struct {

        };
    };
} Ast;

#endif