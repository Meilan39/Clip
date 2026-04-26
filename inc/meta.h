#ifndef META
#define META

#include <stdlib.h>

typedef struct {
    char *alias;
    size_t offset;
    size_t size;
} Node;

typedef struct {
    size_t size;
    Node *nodes;
} Block;

static Block Alias;
static Block Empty;

void meta_cleanup();

#endif // META