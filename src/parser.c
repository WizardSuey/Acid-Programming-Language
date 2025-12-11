#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "debug.h"

Parser parser;


static void parserErrorAt(Token* token, const char* message) {
    fprintf(stderr, "[line %d] Error", token->line);

    if (token->type == TOKEN_EOF) {
        fprintf(stderr, " at end\n");
        parser.hadError = true;
    } else if (token->type == TOKEN_ERROR) {
        fprintf(stderr, ": %s\n", token->start);
        parser.hadError = true;
    } else {
        fprintf(stderr, " at '");
        for (int i = 0; i < token->length; i++) {
            putchar(token->start[i]);
        }
        fprintf(stderr, "': %s\n", message);
        parser.hadError = true;
    }
}

//* Возвращает текущий токен парсера.
static Token* peek() {
    return &parser.current;
}

static Ast* parseExpression(Precedence precedence);

static Ast* parseStatement(void) {
    return parseExpression(PREC_ASSIGNMENT);
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

static Precedence getPrecedence(TokenType type) {
    switch (type) {
        case TOKEN_DOT:
        case TOKEN_LEFT_PAREN:
            return PREC_CALL;
        case TOKEN_BANG:
            return PREC_UNARY;
        case TOKEN_STAR:
        case TOKEN_SLASH:
            return PREC_FACTOR;
        case TOKEN_MINUS:
        case TOKEN_PLUS:
            return PREC_TERM;
        case TOKEN_LESS_LESS:
            return PREC_APPEND;
        case TOKEN_LESS:
        case TOKEN_LESS_EQUAL:
        case TOKEN_GREATER:
        case TOKEN_GREATER_EQUAL:
            return PREC_COMPARISON;
        case TOKEN_EQUAL_EQUAL:
            return PREC_EQUALITY;
        case TOKEN_AND:
            return PREC_AND;
        case TOKEN_OR:
            return PREC_OR;
        case TOKEN_EQUAL:
            return PREC_ASSIGNMENT;

        default:
            return PREC_NONE;
        
    }
}

static void consume(TokenType type, const char* message) {
    if (parser.current.type == type) {
        advance();
        return;
    }

    parserErrorAt(&parser.current, message);
}

static Ast* parsePrimary() {
    Ast* ast;

    advance();
    Token* token = &parser.previous;

    switch (token->type) {
        case TOKEN_NUMBER:
            ast = astBasicValueNew(strtod(token->start, NULL));
            return ast;
        case TOKEN_EOF:
            parserErrorAt(token, "Unexpected end of input.");
            return NULL;
        default:
            parserErrorAt(token, "Unexpected input.");
            return NULL;
    }
    return NULL;
}

static Ast* parsePostfix() {
    Ast* ast;
    AstKind* kind;
    Token* token, *peek;

    if ((ast = parsePrimary()) == NULL) {
        return NULL;
    }

    return ast;
}

static Ast* parseUnary() {
    Token* token = peek();
    
    if (token->type == TOKEN_LEFT_PAREN) {
        advance();
        Ast* ast = parseExpression(PREC_ASSIGNMENT);
        consume(TOKEN_RIGHT_PAREN, "Expected ')' after expression.");
        return ast;
    }

    if (token->type == TOKEN_MINUS ||
        token->type == TOKEN_BANG) {
        AstUnaryOp op;
        switch (token->type) {
            case TOKEN_MINUS: op = AST_UNARY_OP_MINUS; break;
            case TOKEN_BANG: op = AST_UNARY_OP_NOT; break;
            default: break;
        }
        
        advance();

        Ast* operand = parseUnary();

        return astUnaryOpNew(op, operand);
    }
    
    return parsePostfix();
}

static Ast* parseExpression(Precedence precedence) {
    Ast* left;
    Ast* right;
    Token* token;
    Token operatorTokenValue;
    Precedence tokenPrecedence;


    if ((left = parseUnary()) == NULL) {
        return NULL;
    }

    while (1) {
        /* Смотриим на следующий токен */
        token = peek();
        if (token == NULL) {
            return left;
        }

        /* Если следующий токен не является бинарным оператором, завершить разбор */
        if (token->type != TOKEN_MINUS && token->type != TOKEN_PLUS &&
            token->type != TOKEN_SLASH && token->type != TOKEN_STAR &&
            token->type != TOKEN_EQUAL_EQUAL && token->type != TOKEN_BANG_EQUAL &&
            token->type != TOKEN_GREATER && token->type != TOKEN_GREATER_EQUAL &&
            token->type != TOKEN_LESS && token->type != TOKEN_LESS_EQUAL &&
            token->type != TOKEN_AND && token->type != TOKEN_OR &&
            token->type != TOKEN_LESS_LESS &&
            token->type != TOKEN_EQUAL) {
            return left;
        }

        tokenPrecedence = getPrecedence(token->type);
        if (tokenPrecedence == PREC_NONE || tokenPrecedence <= precedence) {
            return left;
        }

        /* Сохранить оператор перед рекурсией (копируем по значению) */
        operatorTokenValue = *token;
        
        /* Потребляем следующий токен */
        advance();
        
        right = parseExpression(tokenPrecedence);

        if (!right) {
            parserErrorAt(&parser.previous, "Expected right-hand expression.");
            return NULL;
        }

        AstBinaryOp op;
        if (!astBinaryOpFromToken(operatorTokenValue.type, &op)) {
            parserErrorAt(&operatorTokenValue, "Invalid binary operator.");
            return NULL;
        }
        if (!left) {
            parserErrorAt(&parser.previous, "Expected left-hand expression.");
            return NULL;
        }
        left = astBinaryOpNew(op, left, right);
    }
}

Ast* parse(void) {
    /* Инициализировать текущие токены перед разбором */
    parser.current = scanToken();
    Ast* ast = parseExpression(PREC_ASSIGNMENT);
    consume(TOKEN_EOF, "Expected end of expression.");
    return ast;
}

