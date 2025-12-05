#ifndef ACID_DSTR_H
#define ACID_DSTR_H

/* Динамические строки */

#include <stddef.h>
#include <stdio.h>
#include "common.h"

typedef struct  dStr{
    char* data;
    size_t length;
    size_t capacity;
} __attribute__((packed)) dStr;

#endif