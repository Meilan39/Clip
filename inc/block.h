#ifndef BLOCK
#define BLOCK

#include <stdlib.h>
#include <string.h>

typedef struct {
    char *alias;
    size_t offset;
    size_t size;
} Node;

typedef struct {
    size_t size;
    Node *nodes;
} Block;

int block_init(Block *block, size_t size);
int node_init(Node *node, const char* alias, size_t offset, size_t size);
void node_free(Node *node);
void block_cleanup(Block *block);

Node* block_next(Block *block, const void *target, int (*compare)(const void *a, const void *b));
void block_sort(Block *block, int (*compare)(const void *a, const void *b));

int compare_alias(const void *a, const void *b);
int compare_offset(const void *a, const void *b);
int compare_size(const void *a, const void *b);

#endif // BLOCK