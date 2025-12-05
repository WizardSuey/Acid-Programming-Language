#include <stdio.h>
#include "debug.h"

void printToken(Token token) {
    printf("Line %d: Token: ", token.line);
    for (int i = 0; i < token.length; i++) {
        putchar(token.start[i]);
    }
    printf("\n");
}

void printAst(Ast* ast) {
    switch (ast->kind) {
        case AST_BASIC_VALUE:
            printf("    Basic value: %f\n", ast->data.AST_BASIC_VALUE.value);
            return;
        case AST_BINARY_OP:
            printf("Binary op (\n");
            printAst(ast->data.AST_BINARY_OP.left);
            switch (ast->data.AST_BINARY_OP.op) {
                case AST_BIN_DIV: printf("    Bin op: / \n"); break;
                case AST_BIN_MUL: printf("    Bin op: * \n"); break;
                case AST_BIN_ADD: printf("    Bin op: + \n"); break;
                case AST_BIN_SUB: printf("    Bin op: - \n"); break;
            }
            printAst(ast->data.AST_BINARY_OP.right);
            printf(")\n");
            return;
    }
}