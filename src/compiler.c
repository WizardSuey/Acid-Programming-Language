#include "compiler.h"
#include "lexer.h"
#include "parser.h"
#include "debug.h"

Parser parser;


bool compile(const char* source) {
    initLexer(source);
    
    parser.hadError = false;


    return !parser.hadError;

}    