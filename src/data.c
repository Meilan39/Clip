#include "../inc/data.h"
#include "../inc/meta.h"
#include <stdio.h>
#include <string.h>

#include "../inc/clipboard.h"

void get_data(const Node* node) {
    FILE *f = fopen(data_path, "rb");
    if (!f) return;
    
    fseek(f, node->offset, SEEK_SET);
    clipboard_set(f, node->size);
    fclose(f);
}

void new_data(const Node* empty, Node* node, const char* segment) {
    FILE *f = fopen(data_path, "r+b");
    if (!f) {
        f = fopen(data_path, "w+b");
        if (!f) return;
    }
    
    if (node->offset == (size_t)-1) {
        fseek(f, 0, SEEK_END);
        node->offset = ftell(f);
    } else {
        fseek(f, node->offset, SEEK_SET);
    }
    
    fwrite(segment, 1, node->size, f);
    fclose(f);
}

void cat_data(const Node* node) {
    FILE *f = fopen(data_path, "rb");
    if (!f) return;
    
    fseek(f, node->offset, SEEK_SET);
    
    char buffer[4096];
    char last_char = '\0';
    size_t remaining = node->size;
    while (remaining > 0) {
        size_t to_read = (remaining < sizeof(buffer)) ? remaining : sizeof(buffer);
        size_t bytes = fread(buffer, 1, to_read, f);
        if (bytes == 0) break;
        fwrite(buffer, 1, bytes, stdout);
        last_char = buffer[bytes - 1];
        remaining -= bytes;
    }
    
    fclose(f);
    if (last_char != '\n') printf("\n");
}

void peek_data(const Node* node) {
    FILE *f = fopen(data_path, "rb");
    if (!f) return;
    
    fseek(f, node->offset, SEEK_SET);
    
    char buffer[101];
    size_t to_read = (node->size < 100) ? node->size : 100;
    size_t bytes = fread(buffer, 1, to_read, f);
    buffer[bytes] = '\0';
    
    printf("%s", buffer);
    if (node->size > 100) {
        printf("...\n");
    } else if (bytes > 0 && buffer[bytes - 1] != '\n') {
        printf("\n");
    }
    
    fclose(f);
}

void move_data(size_t src, size_t dst, size_t size) {
    FILE *f = fopen(data_path, "r+b");
    if (!f) return;
    
    char buffer[4096];
    size_t remaining = size;
    size_t current_src = src;
    size_t current_dst = dst;
    
    while (remaining > 0) {
        size_t to_read = (remaining < sizeof(buffer)) ? remaining : sizeof(buffer);
        
        fseek(f, current_src, SEEK_SET);
        size_t bytes = fread(buffer, 1, to_read, f);
        if (bytes == 0) break;
        
        fseek(f, current_dst, SEEK_SET);
        fwrite(buffer, 1, bytes, f);
        
        current_src += bytes;
        current_dst += bytes;
        remaining -= bytes;
    }
    
    fclose(f);
}
