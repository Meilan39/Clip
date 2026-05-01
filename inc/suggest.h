#ifndef SUGGEST
#define SUGGEST

#include "block.h"

/* init a min heap by distance to node->alias */
size_t heapify(Block *block, const char* str);
void sink(Block *block, size_t i, const char* str);
Node* pop(Block *block, const char* str);

void suggest(Block* block, const char* str);
int distance(const char* a, const char* b);

#endif // SUGGEST