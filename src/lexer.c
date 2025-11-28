#include <stdio.h>
#include <string.h>

#include "common.h"
#include "lexer.h"


typedef struct {
    const char* start;
    const char* current;
    int line;
} Lexer;

Lexer lexer;

void initLexer(const char* source) {
    lexer.start = source;
    lexer.current = source;
    lexer.line = 1;
}

static bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
            c == '_';
}

static bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

static bool isAtEnd() {
    return *lexer.current == '\0';
}

//* Продвигает текущий указатель и возвращает предыдущий символ.
static char advance() {
    lexer.current++;
    return lexer.current[-1];
}

//* Возвращает текущий символ, не продвигая указатель.
static char peek() {
    return *lexer.current;
}

static char peekNext() {
    if (isAtEnd()) return '\0';
    return lexer.current[1];
}

//* Продвигает указатель, если текущий символ совпадает с ожидаемым.
static bool match(char expected) {
    if (isAtEnd()) return false;
    if (*lexer.current != expected) return false;

    lexer.current++;
    return true;
}

static Token makeToken(TokenType type) {
    Token token;
    token.type = type;
    token.start = lexer.start;
    token.length = (int)(lexer.current - lexer.start);
    token.line = lexer.line;
    return token;
}

static Token errorToken(const char* message) {
    Token token;
    token.type = TOKEN_ERROR;
    token.start = message;
    token.length = (int)strlen(message);
    token.line = lexer.line;
    return token;
}

static void skipWhitespace() {
    while (true) {
        char c = peek();
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '\n':
                lexer.line++;
                advance();
                break;
            case '/':
                if (peekNext() == '/') {
                    while (peek() != '\n' && !isAtEnd()) advance();
                } else {
                    return;
                }
                break;
            default:
                return;
        }
    }
}

//* Проверяет, является ли строка ключевым словом.
static TokenType checkKeyword(int start, int length, const char* rest, TokenType type) { 
    if (lexer.current - lexer.start == start + length && memcmp(lexer.start + start, rest, length) == 0) {
        return type;
    }

    return TOKEN_IDENTIFIER;
}

//* Определяет тип идентификатора (ключевое слово или обычный идентификатор).
//* Для реализации этой функции используется Префиксное дерево
static TokenType identifierType() {
    switch (lexer.start[0]) {
        case 'a': return checkKeyword(1, 2, "nd", TOKEN_AND);
        case 'e': 
            if (lexer.current - lexer.start > 1) {
                switch (lexer.start[1]) {
                    case 'l': return checkKeyword(2, 2, "se", TOKEN_ELSE);
                    case 'n': return checkKeyword(2, 1, "d", TOKEN_END);
                }
            }
            break;
        case 'f':
            if (lexer.current - lexer.start > 1) {
                switch (lexer.start[1]) {
                    case 'a': return checkKeyword(2, 3, "lse", TOKEN_FALSE);
                    case 'o': return checkKeyword(2, 1, "r", TOKEN_FOR);
                    case 'u': return checkKeyword(2, 6, "nction", TOKEN_FUNC);
                }
            }
            break;
        case 'i': return checkKeyword(1, 1, "f", TOKEN_IF);
        case 'n': return checkKeyword(1, 3, "ull", TOKEN_NULL);
        case 'o': return checkKeyword(1, 1, "r", TOKEN_OR);
        case 'r': return checkKeyword(1, 5, "eturn", TOKEN_RETURN);
        case 's': return checkKeyword(1, 5, "tdout", TOKEN_STDOUT);
        case 't': return checkKeyword(1, 3, "rue", TOKEN_TRUE);
        case 'v': return checkKeyword(1, 2, "ar", TOKEN_VAR);
        case 'm': return checkKeyword(1, 2, "ut", TOKEN_MUT);
        case 'w': return checkKeyword(1, 4, "hile", TOKEN_WHILE);
    }

    return TOKEN_IDENTIFIER;
}

//* Сканирует и возвращает идентификатор или ключевое слово.
static Token identifier() {
    while (isAlpha(peek()) || isDigit(peek())) advance();
    return makeToken(identifierType());
}

static Token number() {
    while (isDigit(peek())) advance();

    if (peek() == '.' && isDigit(peekNext())) {
        advance(); // Пропустить точку

        while (isDigit(peek())) advance();
    }

    return makeToken(TOKEN_NUMBER);
}

static Token string() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') lexer.line++;
        advance();
    }

    if (isAtEnd()) return errorToken("Unterminated string.");

    advance(); // Закрывающая кавычка
    return makeToken(TOKEN_STRING);
}

Token scanToken() {
    skipWhitespace();

    lexer.start = lexer.current;

    if (isAtEnd()) return makeToken(TOKEN_EOF);

    char c = advance();

    if (isAlpha(c)) return identifier();
    if (isDigit(c)) return number();

    switch (c) {
        case '(': return makeToken(TOKEN_LEFT_PAREN);
        case ')': return makeToken(TOKEN_RIGHT_PAREN);
        case '{': return makeToken(TOKEN_LEFT_BRACE);
        case '}': return makeToken(TOKEN_RIGHT_BRACE);
        case ';': return makeToken(TOKEN_SEMICOLON);
        case ',': return makeToken(TOKEN_COMMA);
        case '.': return makeToken(TOKEN_DOT);
        case '-': return makeToken(TOKEN_MINUS);
        case '+': return makeToken(TOKEN_PLUS);
        case '/': return makeToken(TOKEN_SLASH);
        case '*': return makeToken(TOKEN_STAR);
        case '!':
            return makeToken(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
        case '=':
            return makeToken(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
        case '<':
            return makeToken(match('=') ? TOKEN_LESS_EQUAL : match('<') ? TOKEN_LESS_LESS : TOKEN_LESS);
        case '>':
            return makeToken(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
        case '"': return string();
    }

    return errorToken("Unexpected character.");
}