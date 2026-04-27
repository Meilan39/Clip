#ifndef OUT 
#define OUT 

typedef enum {
    FORMAT,
    META_READ,
    META_WRITE,
} Debug;

void out(Debug debug);

void format();
void file();

#endif // OUT