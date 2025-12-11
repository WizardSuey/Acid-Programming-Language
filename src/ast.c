#include "ast.h"
#include <string.h>


Ast* newAst(Ast ast) {
    Ast* ptr = malloc(sizeof(Ast));
    if (ptr == NULL) {
        fprintf(stderr, "Failed to allocate memory for AST\n");
        exit(74);
    }

    *ptr = ast;
    return ptr;
}

Ast* astBasicValueNew(double value) {
    return NEW_AST(AST_BASIC_VALUE, value);
}

Ast* astStringNew(const char* value, size_t length) {

}

Ast* astBinaryOpNew(AstBinaryOp op, Ast* left, Ast* right) {
    return NEW_AST(AST_BINARY_OP, op, left, right);
}

Ast* astUnaryOpNew(AstUnaryOp op, Ast* expr) {
    return NEW_AST(AST_UNARY_OP, op, expr);
}

int astBinaryOpFromToken(TokenType type, AstBinaryOp* result) {
    switch (type) {
        case TOKEN_PLUS: *result = AST_BIN_ADD; return 1;
        case TOKEN_MINUS: *result = AST_BIN_SUB; return 1;
        case TOKEN_STAR: *result = AST_BIN_MUL; return 1;
        case TOKEN_SLASH: *result = AST_BIN_DIV; return 1;
        case TOKEN_LESS: *result = AST_BIN_LESS; return 1;
        case TOKEN_LESS_EQUAL: *result = AST_BIN_LESS_EQUAL; return 1;
        case TOKEN_GREATER: *result = AST_BIN_GREATER; return 1;
        case TOKEN_GREATER_EQUAL: *result = AST_BIN_GREATER_EQUAL; return 1;
        case TOKEN_EQUAL_EQUAL: *result = AST_BIN_EQUAL; return 1;
        case TOKEN_BANG_EQUAL: *result = AST_BIN_NOT_EQUAL; return 1;
        case TOKEN_AND: *result = AST_BIN_AND; return 1;
        case TOKEN_OR: *result = AST_BIN_OR; return 1;
        case TOKEN_LESS_LESS: *result = AST_BIN_APPEND; return 1;
        case TOKEN_EQUAL: *result = AST_BIN_ASSIGN; return 1;
        default: return 0;
    }
}

void freeAst(Ast* ast) {
    if (ast == NULL) return;
    
    switch (ast->kind) {
        case AST_STRING:
            if (ast->data.AST_STRING.value != NULL) {
                free(ast->data.AST_STRING.value->data);
                free(ast->data.AST_STRING.value);
            }
            break;
        case AST_BINARY_OP:
            freeAst(ast->data.AST_BINARY_OP.left);
            freeAst(ast->data.AST_BINARY_OP.right);
            break;
        case AST_UNARY_OP:
            freeAst(ast->data.AST_UNARY_OP.expr);
            break;
        default:
            break;
    }
    free(ast);
}