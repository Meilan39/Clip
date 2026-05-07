#ifndef CLIPBOARD
#define CLIPBOARD

#include <stdio.h>
#include <stdlib.h>

/*
 * Reads the entire clipboard content into a dynamically allocated buffer.
 * If size is not NULL, stores the size of the read buffer.
 * Returns the buffer on success, or NULL on failure. Caller must free the buffer.
 */
char* clipboard_get(size_t* size);

/*
 * Reads size bytes from the src file stream and writes them to the clipboard.
 */
void clipboard_set(FILE* src, size_t size);

#endif // CLIPBOARD
