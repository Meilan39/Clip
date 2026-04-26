#include "../inc/io.h"

static const char meta_path[] = "../data/meta.txt";
static const char data_path[] = "../data/data.bin";

static char buffer[BUFFER_SIZE];

static FILE* meta = NULL;
static FILE* data = NULL;

int io_init() {
    meta = fopen(meta_path, "r+");
    data = fopen(data_path, "r+");
    if(meta == NULL || data == NULL) {
        out(DATA_NOTFOUND);
        return 1;
    }
    return 0;
}

int io_cleanup() {
    if(meta) fclose(meta);
    if(data) fclose(data);
    return 0;
}

/* handles incorrect data format */
int load() {
    if(read_block(meta, &Alias) == 1)
        goto E;
    if(read_block(meta, &Empty) == 1)
        goto E;

    return 0;
E:  out(DATA_READ);
    return 1;
}

/* handles unable to write */
int store() {
    if(write_block(meta, &Alias))
        goto E;
    if(write_block(meta, &Empty))
        goto E;

    return 0;
E:  out(DATA_WRITE);
    return 1;
}

int read_block(FILE* meta, Block* block) {
    if(fgets(buffer, BUFFER_SIZE, meta) == NULL)
        goto E;

    block->size = atoi(buffer);
    block->nodes = malloc(block->size * sizeof(Node));

    for(int i = 0; i < block->size; i++) 
        if(read_node(meta, &block->nodes[i]))
            goto E;

    return 0;
E:  return 1;
}

int write_block(FILE* meta, Block* block) {
    if(fprintf(meta, "%zu\n", block->size) < 0)
        goto E;

    for(int i = 0; i < block->size; i++)
        if(write_node(meta, &block->nodes[i]))
            goto E;

    return 0;
E:  return 1;
}

int read_node(FILE* meta, Node* node) {       
    char *token;
    char *end;
    if(fgets(buffer, BUFFER_SIZE, meta) == NULL)
        goto E;

    token = strtok(buffer, "|"); // TODO: alias | error
    if(token == NULL) goto E;
    node->size = strlen(token);
    node->alias = malloc(node->size);
    memcpy(node->alias, token, node->size);

    token = strtok(NULL, "|");
    if(token == NULL) goto E;
    node->offset = strtol(token, &end, 10);
    if (*end != '\0') goto E;

    token = strtok(NULL, "|");
    if(token == NULL) goto E;
    node->size = strtol(token, &end, 10);
    if (*end != '\0') goto E;

    return 0;
E:  return 1;
}

int write_node(FILE* meta, Node* node) {
    if (fprintf(meta, "%s,%d,%d\n", 
                       node->alias, node->offset, node->size) < 0)
        goto E;

    return 0;
E:  return 1;
}
