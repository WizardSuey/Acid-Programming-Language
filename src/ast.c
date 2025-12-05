#include "ast.h"


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