#ifndef OUT 
#define OUT 

typedef enum {
    FORMAT,
    META_CORRUPTED,
    META_READ,
    META_WRITE,
    OVERWRITE,
} Debug;

void out(Debug debug);
void format();

#endif // OUT