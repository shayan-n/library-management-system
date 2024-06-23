#include <stdio.h>
#include <stdlib.h>
#include "utilities.h"

#define GLUE_HELPER(x, y) x##y
#define GLUE(x, y) GLUE_HELPER(x, y)

#define T(x) typeof(x)

// defining malloc with 1 len
#define MA(type) \
    (type *) malloc(1 * sizeof(type))

// defining calloc with 1 len
#define CA(type) \
    (type *) calloc(1, sizeof(type))

// defining realloc
#define RA(type, ptr, size) \
    (type *) realloc(ptr, size * sizeof(type))

#define Append(newItem, parent) *(parent->items + parent->length) = newItem; parent->items = RA(T(newItem), parent->items, (parent->length + 2)); parent->length += 1;

#define List(type, name) typedef struct GLUE(List_, name) { int length; type *items; } GLUE(List_, name); GLUE(List_, name) GLUE(List_Object_, name); GLUE(List_, name) *name = &GLUE(List_Object_, name); name->length = 0; name->items = CA(type);