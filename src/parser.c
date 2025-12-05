#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern Parser parser;


static void parserErrorAt(Token* token, const char* message) {
    fprintf(stderr, "[line %d] Error", token->line);

    if (token->type == TOKEN_EOF) {
        fprintf(stderr, " at end");
    } else if (token->type == TOKEN_ERROR) {
        
    } else {
        fprintf(stderr, " at '");
        for (int i = 0; i < token->length; i++) {
            putchar(token->start[i]);
        }
        fprintf(stderr, ": %s'\n", message);
        parser.hadError = true;
    }
}

static Ast* parseStatement(void) {

}

// переключает парсер на следующий токен из сканера и обрабатывает ошибки сканирования
static void advance() {
    parser.previous = parser.current;
    for (;;) {
        parser.current = scanToken();
        if (parser.current.type != TOKEN_ERROR) break;
        parserErrorAt(&parser.current, parser.current.start);
    }
}

Ast* parseUnary() {
    Token* token;
    Ast* ast;

    token = &parser.previous;

    if (token->type == TOKEN_LEFT_PAREN) {
        ast = parseExpression(PREC_ASSIGNMENT);
        consume(TOKEN_RIGHT_PAREN, "Expected ')' after expression.");
        return ast;
    }
}

static Ast* parseExpression(Precedence precedence) {
    Ast* left, right;
    Token* token = &parser.previous;


}

static void consume(TokenType type, const char* message) {
    if (parser.current.type == type) {
        advance();
        return;
    }

    parserErrorAt(&parser.current, message);
}

Ast* parse(void) {
    advance();
    Ast* ast = expression(PREC_ASSIGNMENT);
    consume(TOKEN_EOF, "Expected end of expression.");
    return ast;
}

