#include "../inc/block.h"
#include <string.h>

int block_init(Block *block, size_t size) {
    block->size = size;
    block->nodes = calloc(size, sizeof(Node));
    if(block->nodes == NULL)
        return 1;
    return 0;
}

int node_init(Node *node, const char* alias, size_t offset, size_t size) {
    node->offset = offset;
    node->size = size;
    node->alias = strdup(alias);
    if(node->alias == NULL)
        return 1;
    return 0;
}

void node_free(Node *node) {
    if(node && node->alias) {
        free(node->alias);
        node->alias = NULL;
    }
}

void block_cleanup(Block *block) {
    if(block && block->nodes) {
        for(size_t i = 0; i < block->size; i++) {
            node_free(&block->nodes[i]);
        }
        free(block->nodes);
        block->nodes = NULL;
        block->size = 0;
    }
}

/* binary search (returns pointer to node if found, otherwise the insertion point or NULL) */
Node* block_next(Block *block, const void *target, int (*compare)(const void *a, const void *b)) {
    if (block->size == 0) return NULL;

    int low = 0;
    int high = (int)block->size - 1;
    int mid;

    while (low <= high) {
        mid = low + (high - low) / 2;
        int cmp = compare(&block->nodes[mid], target);
        if (cmp == 0)
            return &block->nodes[mid];
        if (cmp < 0)
            low = mid + 1;
        else
            high = mid - 1;
    }
    
    if (low < (int)block->size) {
        return &block->nodes[low];
    }
    return NULL;
}

void block_sort(Block *block, int (*compare)(const void *a, const void *b)) {
    qsort(block->nodes, block->size, sizeof(Node), compare);
}

int compare_alias(const void *a, const void *b) {
    const Node *node_a = (const Node *)a;
    const Node *node_b = (const Node *)b;
    return strcmp(node_a->alias, node_b->alias);
}

int compare_offset(const void *a, const void *b) {
    const Node *node_a = (const Node *)a;
    const Node *node_b = (const Node *)b;
    if (node_a->offset < node_b->offset) return -1;
    if (node_a->offset > node_b->offset) return 1;
    return 0;
}

int compare_size(const void *a, const void *b) {
    const Node *node_a = (const Node *)a;
    const Node *node_b = (const Node *)b;
    if (node_a->size < node_b->size) return -1;
    if (node_a->size > node_b->size) return 1;
    return 0;
}