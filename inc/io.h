#ifndef IO
#define IO

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "./out.h"
#include "./meta.h"

#define BUFFER_SIZE 1024

static const char meta_path[] = "../data/meta.txt";
static const char data_path[] = "../data/data.bin";

static FILE* meta_r;
static FILE* meta_w;
static FILE* data;

int load();
int store();
int io_init();
int io_cleanup();

int read_block(FILE* meta, Block* block);
int write_block(FILE* meta, Block* block);
int read_node(FILE* meta, Node* node);
int write_node(FILE* meta, Node* node);

#endif // IO