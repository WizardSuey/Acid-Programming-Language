#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "lexer.h"
#include "debug.h"
#include "compiler.h"
#include "ast.h"
#include "memory.h"

#define FILE_EXTENSION ".acid"


static char* readFile(const char* path) {
    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file: %s\n", path);
        exit(74);
    }

    fseek(file, 0L, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);

    char* buffer = (char*)malloc(fileSize + 1);
    if (buffer == NULL) {
        fprintf(stderr, "Failed to allocate memory for file: %s\n", path);
        exit(74);
    }
    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
    if (bytesRead < fileSize) {
        fprintf(stderr, "Failed to read file: %s\n", path);
        exit(74);
    }
    buffer[bytesRead] = '\0';

    fclose(file);
    return buffer;
}

static void runFile(const char* path) {
    const char* extension = strstr(path, FILE_EXTENSION);
    if (!extension || strcmp(extension, FILE_EXTENSION) != 0) {
        fprintf(stderr, "Error: File must have a .acid extension\n");
        exit(112);
    }

    char* source = readFile(path);
    if (!compile(source)) {
        printf("Compilation failed.\n");
        free(source);
        exit(65);
    }
}

int main(int argc, const char* argv[]) {
    if (argc == 2) {
        runFile(argv[1]);
    } else {
        // fprintf(stderr, "Usage: acid [path]\n");
        // exit(64);
        Ast* ast = NEW_AST(AST_BINARY_OP, AST_BIN_ADD, NEW_AST(AST_BASIC_VALUE, 1.0), NEW_AST(AST_BASIC_VALUE, 2.0));
        #ifdef DEBUG_PRINT_AST
            printAst(ast);
        #endif
        FREE(Ast, ast);
    }
    
    return 0;
}