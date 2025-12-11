#include "interpreter.h"
#include "lexer.h"
#include "parser.h"
#include "debug.h"

// #undef DEBUG_PRINT_TOKENS
#undef DEBUG_PRINT_AST

extern Parser parser;


static interpretResult interpretAst(Ast* ast) {
    static void* dispatch_table[] = {
    &&AST_GLOBAL_VAR,
    &&AST_LOCAL_VAR,
    &&AST_FUNC_DECL,
    &&AST_DECL,
    &&AST_BASIC_VALUE,
    &&AST_STRING,
    &&AST_FUNCALL,
    &&AST_IF,
    &&AST_WHILE,
    &&AST_RETURN,
    &&AST_FOR,
    &&AST_BLOCK,
    &&AST_BINARY_OP,
    &&AST_UNARY_OP
};

    #define DISPATCH() goto *dispatch_table[ast->kind]

    #define BINARY_OP(op) \
        do { \
            if ((!IS_AST_BASIC_VALUE(ast->data.AST_BINARY_OP.left)) || (!IS_AST_BASIC_VALUE(ast->data.AST_BINARY_OP.right))) { \
                fprintf(stderr, "Runtime Error: Binary operation requires numeric operands.\n"); \
                return INTERPRET_RUNTIME_ERROR; \
            } \
            double left = ast->data.AST_BINARY_OP.left->data.AST_BASIC_VALUE.value; \
            double right = ast->data.AST_BINARY_OP.right->data.AST_BASIC_VALUE.value; \
            double result; \
            switch (op) { \
                case AST_BIN_ADD: result = left + right; break; \
                case AST_BIN_SUB: result = left - right; break; \
                case AST_BIN_MUL: result = left * right; break; \
                case AST_BIN_DIV: \
                    if (right == 0) { \
                        fprintf(stderr, "Runtime Error: Division by zero.\n"); \
                        return INTERPRET_RUNTIME_ERROR; \
                    } \
                    result = left / right; \
                    break; \
                default: \
                    fprintf(stderr, "Runtime Error: Unknown binary operator.\n"); \
                    return INTERPRET_RUNTIME_ERROR; \
            } \
        } while(false)

    DISPATCH();
    AST_GLOBAL_VAR: {
        return INTERPRET_OK;
    }
    AST_LOCAL_VAR: {
        return INTERPRET_OK;
    }
    AST_FUNC_DECL: {
        return INTERPRET_OK;
    }
    AST_DECL: {
        return INTERPRET_OK;
    }
    AST_BASIC_VALUE: {
        printf("%f\n", ast->data.AST_BASIC_VALUE.value);
        return INTERPRET_OK;
    }
    AST_STRING: {
        printf("%s\n", ast->data.AST_STRING.value->data);
        return INTERPRET_OK;
    }
    AST_FUNCALL: {
        return INTERPRET_OK;
    }
    AST_IF: {
        return INTERPRET_OK;
    }
    AST_WHILE: {
        return INTERPRET_OK;
    }
    AST_RETURN: {
        return INTERPRET_OK;
    }
    AST_FOR: {
        return INTERPRET_OK;
    }
    AST_BLOCK: {
        return INTERPRET_OK;
    }
    AST_BINARY_OP: {
        return INTERPRET_OK;
    }
    AST_UNARY_OP: {
        return INTERPRET_OK;
    }

    #undef BINARY_OP
    #undef DISPATCH
}

bool interpret(const char* source) {
    initLexer(source);
    
    parser.hadError = false;
    
    while (1) {
        Token token = scanToken();
        #ifdef DEBUG_PRINT_TOKENS
            printToken(token);
        #endif
        if (token.type == TOKEN_EOF) break;
    }
    initLexer(source);
    Ast* ast = parse();
    if (ast != NULL) {
        #ifdef DEBUG_PRINT_AST
            printAst(ast);
        #endif

        printf("Result: \n");

        interpretResult result = interpretAst(ast);
        if (result == INTERPRET_RUNTIME_ERROR) return false;

        freeAst(ast);
    }

    return !parser.hadError;

}    