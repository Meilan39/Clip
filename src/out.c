#include "../inc/out.h"

void out(Debug debug) {
    switch(debug) {
        case FORMAT:
            format();
            break;
        case META_CORRUPTED:
            meta_corrupted();
            break;
    }
}

void format() {
    
}

void meta_corrupted() {
    
}

