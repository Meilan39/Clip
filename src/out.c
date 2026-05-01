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
        case RENAME_CONFLICT:
            printf("Error: new alias already exists\n");
            break;
    }
}

void format() {
    printf("Usage: clip <command> [<alias>]\n");
    printf("  get, g    <alias>             : copy data to clipboard\n");
    printf("  new, n    <alias>             : create new data\n");
    printf("  del, d    <alias>             : delete data\n");
    printf("  cat, c    <alias>             : print all data\n");
    printf("  peek, p   <alias>             : print sub-data\n");
    printf("  rename, r <old_alias> <new>   : rename an alias\n");
    printf("  list, ls                      : list all aliases\n");
    printf("  clean                         : clean up deleted data\n");
}
