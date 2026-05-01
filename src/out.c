#include "../inc/out.h"
#include <stdio.h>

void out(Debug debug) {
    switch(debug) {
        case FORMAT:
            format();
            break;
        case META_CORRUPTED:
            printf("Meta file corrupted.\n");
            break;
        case META_READ:
            printf("Error reading meta file\n");
            break;
        case META_WRITE:
            printf("Error writing meta file\n");
            break;
        case OVERWRITE:
            printf("Warning: overwriting existing alias\n");
            break;
    }
}

void format() {
    printf("Usage: clip <command> [<alias>]\n");
    printf("  get <alias>    : copy data to clipboard\n");
    printf("  new <alias>    : create new data\n");
    printf("  del <alias>    : delete data\n");
    printf("  cat <alias>    : print all data\n");
    printf("  peek <alias>   : print sub-data\n");
    printf("  clean          : clean up deleted data\n");
}
