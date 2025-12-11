#include <stdio.h>
#include "debug.h"

void printToken(Token token) {
    printf("Line %d: Token: ", token.line);
    for (int i = 0; i < token.length; i++) {
        putchar(token.start[i]);
    }
    printf(" (Type: %d)", token.type);
    printf("\n");
}

static void printIndent(int depth) {
    for (int i = 0; i < depth; i++) {
        printf("    ");
    }
}

static const char* binOpName(AstBinaryOp op) {
    switch (op) {
        case AST_BIN_DIV: return "/";
        case AST_BIN_MUL: return "*";
        case AST_BIN_ADD: return "+";
        case AST_BIN_SUB: return "-";
        case AST_BIN_LESS: return "<";
        case AST_BIN_LESS_EQUAL: return "<=";
        case AST_BIN_GREATER: return ">";
        case AST_BIN_GREATER_EQUAL: return ">=";
        case AST_BIN_EQUAL: return "==";
        case AST_BIN_NOT_EQUAL: return "!=";
        case AST_BIN_AND: return "and";
        case AST_BIN_OR: return "or";
        case AST_BIN_APPEND: return "<<";
        case AST_BIN_ASSIGN: return "=";
        default: return "<bin-op>";
    }
}

static const char* unaryOpName(AstUnaryOp op) {
    switch (op) {
        case AST_UNARY_OP_MINUS: return "-";
        case AST_UNARY_OP_NOT: return "!";
        default: return "<unary-op>";
    }
}

static void printAstRec(Ast* ast, int depth) {
    if (ast == NULL) {
        printIndent(depth);
        printf("NULL\n");
        return;
    }

    printIndent(depth);
    switch (ast->kind) {
        case AST_BASIC_VALUE:
            printf("BasicValue: %f\n", ast->data.AST_BASIC_VALUE.value);
            return;
        case AST_STRING: {
            printf("String: \"");
            for (size_t i = 0; i < ast->data.AST_STRING.length; i++) putchar(ast->data.AST_STRING.value->data[i]);
            printf("\"\n");
            return;
        }
        case AST_BINARY_OP:
            printf("BinaryOp: %s\n", binOpName(ast->data.AST_BINARY_OP.op));
            /* left */
            printAstRec(ast->data.AST_BINARY_OP.left, depth + 1);
            /* right */
            printAstRec(ast->data.AST_BINARY_OP.right, depth + 1);
            return;
        case AST_UNARY_OP:
            printf("UnaryOp: %s\n", unaryOpName(ast->data.AST_UNARY_OP.op));
            printAstRec(ast->data.AST_UNARY_OP.expr, depth + 1);
            return;
        case AST_FUNCALL:
            printf("FuncCall\n");
            return;
        case AST_IF:
            printf("If\n");
            return;
        case AST_WHILE:
            printf("While\n");
            return;
        case AST_RETURN:
            printf("Return\n");
            return;
        case AST_FOR:
            printf("For\n");
            return;
        case AST_BLOCK:
            printf("Block\n");
            return;
        case AST_LOCAL_VAR:
            printf("LocalVar\n");
            return;
        case AST_GLOBAL_VAR:
            printf("GlobalVar\n");
            return;
        case AST_FUNC_DECL:
            printf("FuncDecl\n");
            return;
        case AST_DECL:
            printf("Decl\n");
            return;
        default:
            printf("AstKind %d\n", ast->kind);
            return;
    }
}

void printAst(Ast* ast) {
    printAstRec(ast, 0);
}
