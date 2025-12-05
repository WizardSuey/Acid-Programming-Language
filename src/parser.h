#ifndef ACID_PARSER_H
#define ACID_PARSER_H

#include "common.h"
#include "lexer.h"
#include "ast.h"

typedef struct {
    Token current;
    Token previous;
    bool hadError;
} Parser;

typedef enum {
    PREC_NONE,
    PREC_ASSIGNMENT,  // =
    PREC_OR,          // or
    PREC_AND,         // and
    PREC_EQUALITY,    // == !=
    PREC_COMPARISON,  // < > <= >=
    PREC_APPEND,      // <<
    PREC_TERM,        // + -
    PREC_FACTOR,      // * /
    PREC_UNARY,       // ! -
    PREC_CALL,        // . ()
    PREC_PRIMARY
} Precedence;

Ast* parse(void);

#endif