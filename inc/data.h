#ifndef DATA
#define DATA

#include <stdlib.h>
#include "block.h"

void move_data(size_t src, size_t dst, size_t size);

void copy_data(const Node* node);
void new_data(const Node* empty, Node* node, const char* segment);
void cat_data(const Node* node);
void peek_data(const Node* node);
char* read_clipboard(size_t* size);

#endif // DATA