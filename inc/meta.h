#ifndef META
#define META

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "./out.h"
#include "./block.h"

#define BUFFER_SIZE 1024

extern Block Alias, Empty;

extern char meta_path[1024];
extern char data_path[1024];

int load_meta();
int store_meta(const Node* idx, const Node* node);
void meta_cleanup();

int read_block(FILE* meta, Block* block);
int write_block(FILE* meta, Block* block, const Node* idx, const Node* node);

#endif // META