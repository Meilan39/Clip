#ifndef OUT 
#define OUT 

typedef enum {
    FORMAT,
    DATA_NOTFOUND,
    DATA_READ,
    DATA_WRITE,
} Debug;

void out(Debug debug);

void format();
void file();

#endif // OUT