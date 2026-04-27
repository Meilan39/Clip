#ifndef META
#define META

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "./out.h"
#include "./block.h"

#define BUFFER_SIZE 1024

static const char meta_path[] = "../data/meta.txt";
static const char data_path[] = "../data/data.bin";

int load_meta();
int store_meta();

int read_block(FILE* meta, Block* block);
int write_block(FILE* meta, Block* block);

#endif // META