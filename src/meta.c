#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "../inc/meta.h"

char meta_path[1024];
char data_path[1024];

static void init_paths() {
    static int initialized = 0;
    if (initialized) return;

    const char* home = getenv("HOME");
    char dir_path[1024];
    if (home) {
        snprintf(dir_path, sizeof(dir_path), "%s/.clip", home);
        snprintf(meta_path, sizeof(meta_path), "%s/.clip/meta.txt", home);
        snprintf(data_path, sizeof(data_path), "%s/.clip/data.bin", home);
    } else {
        strcpy(dir_path, "data");
        strcpy(meta_path, "data/meta.txt");
        strcpy(data_path, "data/data.bin");
    }

    mkdir(dir_path, 0700);
    initialized = 1;
}

Block Alias, Empty;

/* handles incorrect data format */
int load_meta() {
    init_paths();
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
    return -1;
}

/* handles unable to write */
int store_meta(const Node* idx, const Node* node) {
    init_paths();
    FILE* fptr = fopen(meta_path, "w");
    if(fptr == NULL) {
        out(META_WRITE);
        goto E;
    }

    if(write_block(fptr, &Alias, idx, node)) {
        out(META_WRITE);
        goto E;
    }
    /* idx != NULL, node == NULL -> delete node */
    /* delete node -> add it to Empty           */
    const Node* empty_node = (idx != NULL && node == NULL) ? idx : NULL;
    if(write_block(fptr, &Empty, NULL, empty_node)) {
        out(META_WRITE);
        goto E;
    }

    if(fptr) fclose(fptr);
    return 0;
E:  if(fptr) fclose(fptr);
    return -1;
}

void meta_cleanup() {
    block_cleanup(&Alias);
    block_cleanup(&Empty);
}

int read_block(FILE* meta, Block* block) {
    char buffer[BUFFER_SIZE];
    char *token, *end, *alias;
    size_t size, offset;

    /* uninitialized meta files -> initialize size-0 blocks */
    if(fgets(buffer, BUFFER_SIZE, meta) == NULL) {
        block_init(block, 0);
    } else {
        buffer[strcspn(buffer, "\n")] = '\0';
        size = strtol(buffer, &end, 10);
        if (*end != '\0')
            goto E;
        block_init(block, size);
    }

    for(size_t i = 0; i < block->size; i++) {
        /* read line */
        if(fgets(buffer, BUFFER_SIZE, meta) == NULL)
            goto E;
        buffer[strcspn(buffer, "\n")] = '\0';
        
        /* parse alias */
        alias = strtok(buffer, "|");
        if(alias == NULL || strlen(alias) == 0)
            goto E;

        /* parse offset */
        token = strtok(NULL, "|");
        if(token == NULL) goto E;
        offset = strtol(token, &end, 10);
        if (*end != '\0')
            goto E;

        /* parse size */
        token = strtok(NULL, "|");
        if(token == NULL) goto E;
        size = strtol(token, &end, 10);
        if (*end != '\0')
            goto E;

        /* initialize node */
        node_init(&block->nodes[i], alias, offset, size);
    }

    return 0;
E:  return -1;
}

/** write block back with new node
  *     node != NULL
  *         idx != NULL -> insert node at idx (or replace if aliases match)
  *         idx == NULL -> insert node at end
  *     node == NULL
  *         idx == NULL -> do nothing
  *         idx != NULL -> delete node at idx
  */
int write_block(FILE* meta, Block* block, const Node* idx, const Node* node) {
    size_t newsize = block->size;
    if(node != NULL)                newsize++; // insert
    if(node == NULL && idx != NULL) newsize--; // delete
    if(node != NULL && idx != NULL && strcmp(idx->alias, node->alias) == 0) newsize--; // replace
    
    if(fprintf(meta, "%zu\n", newsize) < 0)
        goto E;

    for(size_t i = 0; i < block->size; i++) {
        if(idx == &block->nodes[i]) {
            /* idx != NULL, node == NULL -> skip original */
            if(node == NULL) continue;

            /* idx != NULL, node != NULL -> print new */
            if(fprintf(meta, "%s|%zu|%zu\n", 
                       node->alias,
                       node->offset,
                       node->size) < 0)
                goto E;
            
            /* if replace, print new then skip original */
            if (strcmp(idx->alias, node->alias) == 0) continue;
        }

        /* print original */
        if(fprintf(meta, "%s|%zu|%zu\n", 
                       block->nodes[i].alias,
                       block->nodes[i].offset,
                       block->nodes[i].size) < 0)
            goto E;
    }

    /* idx == NULL, node != NULL -> print new at end */
    if(idx == NULL && node != NULL) {
        if(fprintf(meta, "%s|%zu|%zu\n", 
                       node->alias,
                       node->offset,
                       node->size) < 0)
            goto E;
    }

    return 0;
E:  return -1;
}