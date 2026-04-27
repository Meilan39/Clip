#ifndef BLOCK
#define BLOCK

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

int block_init(Block *block, size_t size);
int node_init(Node *node, const char* alias, size_t offset, size_t size);
int block_cleanup();

int search(Block *block, Node* node, int (*comp)(const Node*, const Node*));
int sort(Block *block, int (*comp)(const Node*, const Node*));

int compare_alias(const Node *a, const Node *b);
int compare_offset(const Node *a, const Node *b);
int compare_size(const Node *a, const Node *b);

#endif // BLOCK