#include "../inc/meta.h"

void meta_cleanup() {
    if(Alias.nodes) {
        for(int i = 0; i < Alias.size; i++) {
            Node* node = &Alias.nodes[i];
            if(node->alias)
                free(node->alias);
        }
    }
    if(Empty.nodes) {
        for(int i = 0; i < Empty.size; i++) {
            Node* node = &Empty.nodes[i];
            if(node->alias)
                free(node->alias);
        }
    }
}