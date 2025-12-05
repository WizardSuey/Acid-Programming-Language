#ifndef ACID_DEBUG_H
#define ACID_DEBUG_H

#include "lexer.h"
#include "ast.h"

void printToken(Token token);
void printAst(Ast* ast);

#endif