#include "./out.h"

void out(Debug debug) {
    switch(debug) {
        case FORMAT:
            format();
            break;
        case FILE:
            file();
            break;
    }
}

