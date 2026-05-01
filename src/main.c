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
void clean();

int main(int argc, char *argv[]) {
    if(argc == 2)
    {
        if(strcmp(argv[1], "clean") == 0) {  // clip clean
            clean();
        }
        else {                          // clip ~alias~
            get(argv[1]);
        }
    } else
    if(argc == 3) 
    {
        if(strcmp(argv[1], "get") == 0) {    // clip get ~alias~
            get(argv[2]); 
        } else
        if(strcmp(argv[1], "new") == 0) {    // clip new ~alias~
            new(argv[2]); 
        } else
        if(strcmp(argv[1], "delete") == 0) { // clip delete ~alias~
            delete(argv[2]); 
        } else
        if(strcmp(argv[1], "cat") == 0) {    // clip cat ~alias~
            cat(argv[2]);
        } else
        if(strcmp(argv[1], "peek") == 0) {   // clip peek ~alias~
            peek(argv[2]);
        } 
        else {                          // formatting error
            out(FORMAT);
        }
    } 
    else {                              // formatting error
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