#include "../inc/suggest.h"
#include <string.h>
#include <stdio.h>

/* Damerau–Levenshtein distance */
int distance(const char* a, const char* b) {
    int len_a = strlen(a);
    int len_b = strlen(b);
    if (len_a == 0) return len_b;
    if (len_b == 0) return len_a;

    // Use VLA since alias lengths are typically small
    int d[len_a + 1][len_b + 1];

    for (int i = 0; i <= len_a; i++) d[i][0] = i;
    for (int j = 0; j <= len_b; j++) d[0][j] = j;

    for (int i = 1; i <= len_a; i++) {
        for (int j = 1; j <= len_b; j++) {
            int cost = (a[i-1] == b[j-1]) ? 0 : 1;
            int min = d[i-1][j] + 1; // deletion
            if (d[i][j-1] + 1 < min) min = d[i][j-1] + 1; // insertion
            if (d[i-1][j-1] + cost < min) min = d[i-1][j-1] + cost; // substitution

            if (i > 1 && j > 1 && a[i-1] == b[j-2] && a[i-2] == b[j-1]) {
                if (d[i-2][j-2] + cost < min) min = d[i-2][j-2] + cost; // transposition
            }
            d[i][j] = min;
        }
    }
    return d[len_a][len_b];
}

/* Helper to compute the score of a node against the target string */
static int node_score(const Node* node, const char* str) {
    int penalty = 0;
    const char* slash = strchr(str, '/');
    if (slash != NULL) {
        size_t prefix_len = slash - str;
        // those with matching substring preceding a '/' are oriented first
        if (strncmp(node->alias, str, prefix_len) == 0 && node->alias[prefix_len] == '/') {
            // matches prefix, no penalty
        } else {
            // max DL distance is roughly max string length, so 1000 is a safe large penalty
            penalty = 1000;
        }
    }
    return penalty + distance(node->alias, str);
}

void sink(Block *block, size_t i, const char* str) {
    size_t size = block->size;
    while (2 * i + 1 < size) {
        size_t left = 2 * i + 1;
        size_t right = 2 * i + 2;
        size_t smallest = left;

        // Compare children
        if (right < size && node_score(&block->nodes[right], str) < node_score(&block->nodes[left], str)) {
            smallest = right;
        }

        // If parent is smaller than or equal to smallest child, we're done
        if (node_score(&block->nodes[i], str) <= node_score(&block->nodes[smallest], str)) {
            break;
        }

        // Swap parent with smallest child
        Node temp = block->nodes[i];
        block->nodes[i] = block->nodes[smallest];
        block->nodes[smallest] = temp;

        i = smallest;
    }
}

size_t heapify(Block *block, const char* str) {
    size_t prefix_matches = 0;
    const char* slash = strchr(str, '/');
    size_t prefix_len = slash ? (size_t)(slash - str) : 0;

    // Count prefix matches
    for (size_t i = 0; i < block->size; i++) {
        if (slash && strncmp(block->nodes[i].alias, str, prefix_len) == 0 && block->nodes[i].alias[prefix_len] == '/') {
            prefix_matches++;
        }
    }

    // Build min-heap (Floyd's algorithm)
    for (int i = (int)block->size / 2 - 1; i >= 0; i--) {
        sink(block, (size_t)i, str);
    }

    size_t min = (prefix_matches > 5) ? prefix_matches : 5;
    if (min > block->size) {
        min = block->size;
    }
    return min;
}

/* pop minimum element of min-heap and call sink */
Node* pop(Block *block, const char* str) {
    if (block->size == 0) return NULL;

    // Swap root with the last element
    size_t last = block->size - 1;
    Node temp = block->nodes[0];
    block->nodes[0] = block->nodes[last];
    block->nodes[last] = temp;

    // Decrease heap size
    block->size--;

    // Sink the new root
    if (block->size > 0) {
        sink(block, 0, str);
    }

    // Return the element that was moved to the end
    return &block->nodes[last];
}

void suggest(Block* block, const char* str) {
    printf("Did you mean ...\n");
    size_t min = heapify(block, str);
    for(size_t i = 0; i < min; i++) {
        Node *popped = pop(block, str);
        if (popped) {
            printf("  %s\n", popped->alias);
        }
    }
}