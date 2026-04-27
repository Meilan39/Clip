#include <string.h>
#include "../inc/out.h"
#include "../inc/io.h"

void get(const char* str);
void new(const char* str);
void delete(const char* str);
void peek(const char* str);

void init();
void cleanup();

int main(int argc, char *argv[]) {

    if(argc == 2)
    {
        if(strcmp(argv[1], "clean")) {  // clip clean
            clean();
        }
        else {                          // clip ~alias~
            get(argv[1]);
        }
    } else
    if(argc == 3) 
    {
        if(strcmp(argv[1], "get")) {    // clip get ~alias~
            get(argv[1]); 
        } else
        if(strcmp(argv[1], "new")) {    // clip new ~alias~
            new(argv[1]); 
        } else
        if(strcmp(argv[1], "delete")) { // clip delete ~alias~
            delete(argv[1]); 
        } else
        if(strcmp(argv[1], "peek")) {   // clip peek ~alias~
            peek(argv[1]);
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
    if(load_meta() == 1) goto E;
    if(search()) 
    if(store() == 1) goto E;

    return;
E:  block_cleanup();
    return;
}


void new(const char* str);
void delete(const char* str);
void peek(const char* str);

void init() {
    if(io_init() == 1)
       goto E;

    return;
E:  cleanup();
}

void cleanup() {
    io_cleanup();
    meta_cleanup();
}