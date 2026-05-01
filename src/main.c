#include <string.h>
#include "../inc/out.h"
#include "../inc/meta.h"
#include "../inc/data.h"
#include "../inc/block.h"
#include "../inc/suggest.h"

void get(const char* str);
void new(const char* str);
void delete(const char* str);
void cat(const char* str);
void peek(const char* str);
void renm(const char* old, const char* new);
void list();
void clean();

int main(int argc, char *argv[]) {
    if(argc == 2)
    {
        if(strcmp(argv[1], "clean") == 0) { 
            clean();
        } else
        if(strcmp(argv[1], "list") == 0 || strcmp(argv[1], "ls") == 0) { 
            list();
        }
        else {                         
            get(argv[1]);
        }
    } else
    if(argc == 3) 
    {
        if(strcmp(argv[1], "get") == 0 || strcmp(argv[1], "g") == 0) {    
            get(argv[2]); 
        } else
        if(strcmp(argv[1], "new") == 0 || strcmp(argv[1], "n") == 0) {    
            new(argv[2]); 
        } else
        if(strcmp(argv[1], "delete") == 0 || strcmp(argv[1], "d") == 0) { 
            delete(argv[2]); 
        } else
        if(strcmp(argv[1], "cat") == 0 || strcmp(argv[1], "c") == 0) {   
            cat(argv[2]);
        } else
        if(strcmp(argv[1], "peek") == 0 || strcmp(argv[1], "p") == 0) {  
            peek(argv[2]);
        } else {                          
            out(FORMAT);
        }
    } 
    else if(argc == 4) 
    {
        if(strcmp(argv[1], "renm") == 0 || strcmp(argv[1], "r") == 0) {  
            renm(argv[2], argv[3]);
        } 
        else {                          
            out(FORMAT);
        }
    }
    else {                              
        out(FORMAT);
    }
}

void get(const char* str) {
    Node *next;
    Node target = {0};

    if(load_meta() == -1) goto E;

    target.alias = (char*)str;
    next = block_next(&Alias, &target, compare_alias);

    if(next && strcmp(next->alias, str) == 0) {
        copy_data(next);
    } else {
        suggest(&Alias, str);
    }

E:  meta_cleanup();
    return;
}

void new(const char* str) {
    Node *next, *empty;
    Node target = {0};
    char* segment = NULL;
    size_t seg_size = 0;

    if(load_meta() == -1) goto E;

    if((segment = read_clipboard(&seg_size)) == NULL)
        goto E;

    target.alias = (char*)str;
    target.size  = seg_size;
    next = block_next(&Alias, &target, compare_alias);

    /* if alias already exist, warn overwrite */
    if(next && strcmp(next->alias, str) == 0) 
        out(OVERWRITE);

    block_sort(&Empty, compare_size);
    empty = block_next(&Empty, &target, compare_size);

    if(empty != NULL) { // take from empty
        target.offset =  empty->offset;
        empty->offset += target.size;
        empty->size   -= target.size;
    } else {            // append at end
        target.offset = (size_t)-1; // append to end
    }

    new_data(empty, &target, segment); // allocate new data segement

    if(store_meta(next, &target) == -1) goto E;

E:  if (segment) free(segment);
    meta_cleanup();
    return; 
}

void delete(const char* str) {
    Node *next;
    Node target = {0};

    if(load_meta() == -1) goto E;

    target.alias = (char*)str;
    next = block_next(&Alias, &target, compare_alias);

    if(next && strcmp(next->alias, str) == 0) {
        if(store_meta(next, NULL) == -1) goto E;
    } else {
        suggest(&Alias, str);
    }

E:  meta_cleanup();
    return;
}

void cat(const char* str) {
    Node *next;
    Node target = {0};

    if(load_meta() == -1) goto E;

    target.alias = (char*)str;
    next = block_next(&Alias, &target, compare_alias);

    if(next && strcmp(next->alias, str) == 0) {
        cat_data(next);
    } else {
        suggest(&Alias, str);
    }

E:  meta_cleanup();
    return;
}

void peek(const char* str) {
    Node *next;
    Node target = {0};

    if(load_meta() == -1) goto E;

    target.alias = (char*)str;
    next = block_next(&Alias, &target, compare_alias);

    if(next && strcmp(next->alias, str) == 0) {
        peek_data(next);
    } else {
        suggest(&Alias, str);
    }

E:  meta_cleanup();
    return;
}

void renm(const char* old, const char* new) {
    Node *next;
    Node target = {0};
    Node check = {0};

    if(load_meta() == -1) goto E;

    /* check if new alias already exist */
    check.alias = (char*)new;
    next = block_next(&Alias, &check, compare_alias);
    if(next && strcmp(next->alias, new) == 0) {
        out(RENAME_CONFLICT);
        goto E; 
    }

    target.alias = (char*)old;
    next = block_next(&Alias, &target, compare_alias);

    if(next && strcmp(next->alias, old) == 0) {
        free(next->alias);
        next->alias = strdup(new);
        block_sort(&Alias, compare_alias);
        /* write back with update */
        if(store_meta(NULL, NULL) == -1) goto E;
    } else {
        suggest(&Alias, old);
    }

E:  meta_cleanup();
    return;   
}

void clean() {
    if(load_meta() == -1) goto E;

    block_sort(&Alias, compare_offset);

    size_t tail = 0;
    for(size_t i = 0; i < Alias.size; i++) {
        Node* next = &Alias.nodes[i];
        if(next->offset != tail) { 
            /* fill the gap */
            move_data(next->offset, tail, next->size);
            next->offset = tail;
        }
        tail += next->size;
    }

    block_sort(&Alias, compare_alias);
    
    block_cleanup(&Empty);
    block_init(&Empty, 0);

    if(store_meta(NULL, NULL) == -1) goto E;

E:  meta_cleanup();
    return;
}

void list() {
    if(load_meta() == -1) goto E;

    for(size_t i = 0; i < Alias.size; i++) {
        Node* next = &Alias.nodes[i];
        printf("  %s\n", next->alias);
    }

E:  meta_cleanup();
    return;
}
