#include "../inc/clipboard.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(__APPLE__)
    #define CLIP_COPY_CMD "pbcopy"
    #define CLIP_PASTE_CMD "pbpaste"
#elif defined(_WIN32)
    #define CLIP_COPY_CMD "clip"
    #define CLIP_PASTE_CMD "powershell -command \"Get-Clipboard\""
#else
    #define CLIP_COPY_CMD "xclip -selection clipboard"
    #define CLIP_PASTE_CMD "xclip -selection clipboard -o"
#endif

char* clipboard_get(size_t* size) {
    FILE *pb = popen(CLIP_PASTE_CMD, "r");
    if (!pb) return NULL;
    
    size_t capacity = 1024;
    char *buffer = malloc(capacity);
    size_t len = 0;
    
    while (1) {
        if (len + 1024 > capacity) {
            capacity *= 2;
            buffer = realloc(buffer, capacity);
        }
        size_t bytes = fread(buffer + len, 1, 1024, pb);
        if (bytes == 0) break;
        len += bytes;
    }
    
    buffer[len] = '\0';
    if (size) *size = len;
    
    pclose(pb);
    return buffer;
}

void clipboard_set(FILE* src, size_t size) {
    FILE *pb = popen(CLIP_COPY_CMD, "w");
    if (!pb) return;
    
    char buffer[4096];
    size_t remaining = size;
    while (remaining > 0) {
        size_t to_read = (remaining < sizeof(buffer)) ? remaining : sizeof(buffer);
        size_t bytes = fread(buffer, 1, to_read, src);
        if (bytes == 0) break;
        fwrite(buffer, 1, bytes, pb);
        remaining -= bytes;
    }
    
    pclose(pb);
}
