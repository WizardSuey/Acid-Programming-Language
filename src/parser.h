#ifndef ACID_PARSER_H
#define ACID_PARSER_H

#include "common.h"
#include "lexer.h"

typedef struct {
    Token current;
    Token previous;
    bool hadError;
} Parser;

#endif