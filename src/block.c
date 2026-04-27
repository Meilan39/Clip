#include "../inc/meta.h"

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
    node->alias = calloc(strlen(alias) + 1, sizeof(char));
    if(node->alias == NULL)
        return 1;
    strcpy(node->alias, alias);
    return 0;
}

void block_cleanup() {
    if(Alias.nodes) {
        for(int i = 0; i < Alias.size; i++) {
            Node* node = &Alias.nodes[i];
            if(node->alias)
                free(node->alias);
        }
    }
    if(Empty.nodes) {
        for(int i = 0; i < Empty.size; i++) {
            Node* node = &Empty.nodes[i];
            if(node->alias)
                free(node->alias);
        }
    }
}

/* binary search (return node greater or equal)*/
int search(Block *block, Node* node, int (*comp)(const Node*, const Node*)) {
    size_t low = 0, high = block->size;
    while(low < high) {
        size_t mid = low + (high - low) / 2;
        int cmp = comp(block->nodes + mid, node);
        if(cmp == 0) {
            *node = block->nodes[mid];
            return 0;
        }
        else if(cmp < 0)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return 0;
}

int sort(Block *block, int (*comp)(const Node*, const Node*)) {
    return 0;
}

int compare_alias(const Node *a, const Node *b) {
    return strcmp(a->alias, b->alias);
}

int compare_offset(const Node *a, const Node *b) {
    return a->offset > b->offset ? 1 : -1;
}

int compare_size(const Node *a, const Node *b) {
    return a->size > b->size ? 1 : -1;
}

