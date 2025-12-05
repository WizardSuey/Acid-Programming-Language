#ifndef ACID_AST_H
#define ACID_AST_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "dstr.h"
#include "common.h"


typedef enum AstUnaryOp {
    AST_UNARY_OP_MINUS,    /* -x */
    AST_UNARY_OP_NOT       /* !x */
} AstUnaryOp;

typedef enum AstBinaryOp {
    AST_BIN_MUL,
    AST_BIN_DIV,
    AST_BIN_ADD,
    AST_BIN_SUB,

    AST_BIN_LESS,
    AST_BIN_LESS_EQUAL,
    AST_BIN_GREATER,
    AST_BIN_GREATER_EQUAL,

    AST_BIN_EQUAL,
    AST_BIN_NOT_EQUAL,

    AST_BIN_AND,           /* x and y */
    AST_BIN_OR,            /* x or y */
    AST_BIN_APPEND,        /* x << y */

    AST_BIN_ASSIGN         /* x = y */
} AstBinaryOp;

typedef enum AstKind {
    AST_GLOBAL_VAR,
    AST_LOCAL_VAR,
    AST_FUNC_DECL,
    AST_DECL,
    AST_BASIC_VALUE,
    AST_STRING,
    AST_FUNCALL,
    AST_IF,
    AST_WHILE,
    AST_RETURN,
    AST_FOR,
    AST_BLOCK,
    AST_BINARY_OP,
    AST_UNARY_OP
} AstKind;

typedef struct Ast Ast;
typedef struct Ast{
    AstKind kind;
    union {
        struct AST_BASIC_VALUE {               /* Basic value (number) */
            double value;
        } AST_BASIC_VALUE;

        struct {               /* String */
            dStr *value;
            size_t length;
        } AST_STRING;

        struct AST_BINARY_OP {               /* Binary operation */
            AstBinaryOp op;
            Ast* left;
            Ast* right;
        } AST_BINARY_OP;

        struct AST_UNARY_OP {               /* Unary operation */
            AstUnaryOp op;
            Ast* expr;
        } AST_UNARY_OP;
    } data;
} Ast;

Ast* newAst(Ast ast);

Ast* astBasicValueNew(double value);
Ast* astStringNew(const char* value, size_t length);
Ast* astBinaryOpNew(AstBinaryOp op, Ast* left, Ast* right);
Ast* astUnaryOpNew(AstUnaryOp op, Ast* expr);

#define NEW_AST(kind, ...) \
    newAst((Ast){kind, {.kind=(struct kind){__VA_ARGS__}}})

#endif
