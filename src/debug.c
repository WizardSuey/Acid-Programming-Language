#include <stdio.h>
#include "debug.h"

void printToken(Token token) {
    printf("Line %d: Token: ", token.line);
    for (int i = 0; i < token.length; i++) {
        putchar(token.start[i]);
    }
    printf("\n");
}