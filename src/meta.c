#include "../inc/meta.h"

/* handles incorrect data format */
int load_meta() {
    FILE* fptr = fopen(meta_path, "r");

    if(fptr == NULL) {
        block_init(&Alias, 0);
        block_init(&Empty, 0);
    } else {
        if(read_block(fptr, &Alias)) {
            out(META_READ);
            goto E;
        }
        if(read_block(fptr, &Empty)) {
            out(META_READ);
            goto E;
        }
    }

    if(fptr) fclose(fptr);
    return 0;
E:  if(fptr) fclose(fptr);
    return 1;
}

/* handles unable to write */
int store_meta() {
    FILE* fptr = fopen(meta_path, "w");
    if(fptr == NULL) {
        out(META_WRITE);
        goto E;
    }

    if(write_block(fptr, &Alias)) {
        out(META_WRITE);
        goto E;
    }
    if(write_block(fptr, &Empty)) {
        out(META_WRITE);
        goto E;
    }

    if(fptr) fclose(fptr);
    return 0;
E:  if(fptr) fclose(fptr);
    return 1;
}

int read_block(FILE* meta, Block* block) {
    char buffer[BUFFER_SIZE];
    char *token, *end, *alias;
    size_t size, offset;

    /* read header */
    if(fgets(buffer, BUFFER_SIZE, meta) == NULL)
        goto E;
    buffer[strcspn(buffer, "\n")] = '\0';

    size = strtol(buffer, &end, 10);
    if (*end != '\0')
        goto E;
    block_init(block, size);

    for(size_t i = 0; i < block->size; i++) {
        /* read line */
        if(fgets(buffer, BUFFER_SIZE, meta) == NULL)
            goto E;
        buffer[strcspn(buffer, "\n")] = '\0';
        
        /* parse alias */
        alias = strtok(buffer, "|");
        if(strlen(alias) == 0)
            goto E;

        /* parse offset */
        token = strtok(NULL, "|");
        offset = strtol(token, &end, 10);
        if (*end != '\0')
            goto E;

        /* parse size */
        token = strtok(NULL, "|");
        size = strtol(token, &end, 10);
        if (*end != '\0')
            goto E;

        /* initialize node */
        node_init(&block->nodes[i], alias, offset, size);
    }

    return 0;
E:  return 1;
}

int write_block(FILE* meta, Block* block) {
    if(fprintf(meta, "%zu\n", block->size) < 0)
        goto E;

    for(size_t i = 0; i < block->size; i++) {
        if(fprintf(meta, "%s|%zu|%zu\n", 
                       block->nodes[i].alias,
                       block->nodes[i].offset,
                       block->nodes[i].size) < 0)
            goto E;
    }

    return 0;
E:  return 1;
}