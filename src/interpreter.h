#ifndef ACID_INTERPRETER_H
#define ACID_INTERPRETER_H

#include "common.h"
#include "ast.h"


typedef enum {
    INTERPRET_OK,
    INTERPRET_RUNTIME_ERROR
} interpretResult;

bool interpret(const char* source);
void freeAst(Ast* ast);

#endif