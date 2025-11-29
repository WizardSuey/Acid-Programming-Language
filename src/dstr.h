#ifndef ACID_DSTR_H
#define ACID_DSTR_H

/* Динамические строки */

#include <stddef.h>

typedef struct {
    char* data;
    size_t length;
    size_t capacity;
} dStr;

#endif